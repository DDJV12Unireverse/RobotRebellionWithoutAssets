// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "DroneAIController.h"
#include "Character/King.h"
#include "Character/RobotRebellionCharacter.h"
#include "Character/NonPlayableCharacter.h"
#include "Character/PlayableCharacter.h"
#include "Character/Drone.h"
#include "Components/SplineComponent.h"
#include "IA/Navigation/NavigationVolumeGraph.h"


#define VERY_LITTLE 5.0f

ADroneAIController::ADroneAIController() : ACustomAIControllerBase()
{
    PrimaryActorTick.bCanEverTick = true;

    m_splinePath = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
    m_splinePath->DefaultUpVector = {0.f, 0.f, 1.f};
    m_splinePath->ScaleVisualizationWidth = 30.f;
    m_splinePath->SetSelectedSplineSegmentColor(FLinearColor::Blue);
    m_splinePath->bShouldVisualizeScale = true;
    m_splinePath->bAllowDiscontinuousSpline = false;

    m_idleTimer = 0.f;
    m_actionFinished = true;

    m_king = nullptr;
}

void ADroneAIController::BeginPlay()
{
    Super::BeginPlay();

    m_bestBombLocation = FVector::ZeroVector;

    m_currentTime = 0.f;

    m_state = DRONE_MOVING; //for testing
    m_coeffKing = 3.f;
}

void ADroneAIController::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    this->updateFrameProperties(deltaTime);

    this->IAUpdate(deltaTime);
}

void ADroneAIController::updateFrameProperties(float deltaTime)
{
    m_currentTime += deltaTime;
    m_timeSinceLastUpdate = deltaTime;

    this->updateEnnemiesCampInfo();
    this->updateAlliesCampInfo();
}

void ADroneAIController::updateEnnemiesCampInfo()
{
    m_ennemyInScene = 0;
    m_ennemyNear = 0;
    m_ennemyNearBarycenter = FVector::ZeroVector;

    const float squaredDetectRange = m_detectionDistance * m_detectionDistance;

    TArray<AActor*> npcCharact;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANonPlayableCharacter::StaticClass(), npcCharact);

    int32 npcCount = npcCharact.Num();
    for(int32 iter = 0; iter < npcCount; ++iter)
    {
        AActor* current = npcCharact[iter];

        AKing* king = Cast<AKing>(current);
        if(king)
        {
            m_king = king;
            continue;
        }

        ADrone* drone = Cast<ADrone>(current);
        if(!drone)
        {
            ++m_ennemyInScene;
            FVector ennemyPosition = GetPawn()->GetActorLocation();

            if (FVector::DistSquared(current->GetActorLocation(), ennemyPosition) < squaredDetectRange)
            {
                ++m_ennemyNear;
                m_ennemyNearBarycenter += ennemyPosition;
            }
        }
    }

    if (m_ennemyNear != 0)
    {
        m_ennemyNearBarycenter /= m_ennemyNear;
    }
    else
    {
        m_ennemyNearBarycenter = GetPawn()->GetActorLocation();
    }
}

void ADroneAIController::updateAlliesCampInfo()
{
    m_alliesAliveCount = 0;
    m_alliesInScene = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();
    m_groupBarycenter = FVector::ZeroVector;

    for(int noplayer = 0; noplayer < m_alliesInScene; ++noplayer)
    {
        APlayableCharacter* currentPlayer = Cast<APlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), noplayer));
        if(!currentPlayer->isDead())
        {
            ++m_alliesAliveCount;
            m_groupBarycenter += currentPlayer->GetActorLocation();
        }
    }

    if (m_alliesAliveCount != 0)
    {
        m_groupBarycenter /= m_alliesAliveCount;
    }
    else
    {
        m_groupBarycenter = GetPawn()->GetActorLocation();
    }
}

void ADroneAIController::receiveBomb()
{
    if(Role >= ROLE_Authority)
    {
        ADrone * drone = Cast<ADrone>(this->GetPawn());
        if(drone)
        {
            drone->reload();
            //m_gotBomb = true;
        }
        return;
    }
    serverReceiveBomb();
}

void ADroneAIController::serverReceiveBomb_Implementation()
{
    ADrone * drone = Cast<ADrone>(this->GetPawn());
    if(drone)
    {
        drone->reload();
    }
    //m_gotBomb = true;
}
bool ADroneAIController::serverReceiveBomb_Validate()
{
    return true;
}
bool ADroneAIController::HasABomb()
{
    return Cast<ADrone>(GetPawn())->isLoaded();
    //return m_gotBomb;
}


float ADroneAIController::getAttackScore()
{
    ADrone* owner = Cast<ADrone>(this->GetPawn());
    CheckEnnemyNear(m_detectionDistance);

    float score{};

    if(!owner->isLoaded() || m_sensedEnnemies.Num() == 0)
    { // Cannot attack cause no bomb
        return 0.f;
    }
    //Test all positions
    float bestScoreBomb{};
    int32 indexBestBomb{};
    for(int i = 0; i < m_sensedEnnemies.Num(); i++)
    {
        FVector ennemyPos = m_sensedEnnemies[i]->GetActorLocation();
        float bombScore = getBombScore(ennemyPos);
        if(bombScore > bestScoreBomb)
        {
            bestScoreBomb = bombScore;
            indexBestBomb = i;
        }
    }
    m_bestBombLocation = m_sensedEnnemies[indexBestBomb]->GetActorLocation();

    const float c_Normalize = 4.f; // 4 additions
    const float c_NbPlayersMax = 4.f; // 4 elements

    score = 0.6f * bestScoreBomb;
    float temp = m_alliesAliveCount * 3.f;
    temp = temp == 0 ? 1.f : temp;
    temp = (m_sensedEnnemies.Num() / temp);
    temp = temp > 1 ? 1.f : temp;
    score += 0.3f * temp;
    if(getNbBombPlayers())
    {
        score += 0.1f; // Add 20%
    }

    return score;
}

float ADroneAIController::getFollowScore()
{
    float score;
    score = 1 - 250000.f / (0.1f + (GetPawn()->GetActorLocation() - m_groupBarycenter).SizeSquared()); //Change later
    score = (score < 0.f) ? 0.f : score;
    score *= score;
    if(isInCombat())
    {
        score *= score;
    }

    return score;
}

float ADroneAIController::getReloadScore()
{
    float score = 1.0f;
    // No Bomb
    if(Cast<ADrone>(this->GetPawn())->isLoaded() || (getNbBombPlayers() == 0))  // no reloading possible
    {
        score = 0.0f;
    }
    else
    {
        FVector dronePosition = GetPawn()->GetActorLocation();
        m_safeZone = findSafeZone();
        if(isInCombat())
        {
            //if not in combat, combat score always =1, else it's closer to 1 if many ennemies
            score *= (1 - (m_alliesAliveCount / (4 * m_ennemyNear)));
        }
        else
        {
            score *= FVector::DistSquared(m_groupBarycenter, dronePosition)
                / FVector::DistSquared(m_safeZone, dronePosition);
        }
        score *= (1 - getNbEnnemiesInZone(m_safeZone) / (0.1f + FVector::Dist(dronePosition, m_safeZone))); //ZoneScore
        score = (score < 0.f) ? 0.f : score;
    }

    return score;
}

float ADroneAIController::getWaitingScore()
{
    /*ADrone* drone = Cast<ADrone>(GetPawn());
    return 1.f - getAttackScore();*/
    return m_waitingThreshold;
}

float ADroneAIController::getDropScore()
{
    float score = 0.f;

    ADrone* drone = Cast<ADrone>(this->GetPawn());
    FVector dronePosition = drone->GetActorLocation();

    //PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("DROP DISTANCE: %f"), FVector(dronePosition - m_destination).Size()));

    if(m_state == DRONE_COMBAT && this->isArrivedAtDestination() && drone->isLoaded())
    {
        score = getBombScore(m_bestBombLocation);
    }
    return score;
}


bool ADroneAIController::isInCombat()
{
    //  TODO - Check if ennemy are attacking players or players attacking ennemy
    return (m_ennemyNear > 0);
}

int ADroneAIController::getNbEnnemiesInZone(const FVector& zoneCenter)
{
    CheckEnnemyNearPosition(zoneCenter, m_detectionDistance);
    return (m_sensedEnnemies.Num());
}

EPathFollowingRequestResult::Type ADroneAIController::MoveToTarget()
{
    ADrone* owner = Cast<ADrone>(GetPawn());

    if(m_finalPath.Num() == 0)
    {
        owner->GetMovementComponent()->Velocity = FVector::ZeroVector;
        return EPathFollowingRequestResult::AlreadyAtGoal;
    }

    FVector actorLocation = owner->GetActorLocation();

    FVector directionToTarget = m_finalPath.Top() - actorLocation;
    FVector lastDirection = owner->GetMovementComponent()->Velocity;

    // Check if we have reach the current point
    while(
        m_finalPath.Num() != 0 &&
        (FVector::DotProduct(directionToTarget, lastDirection) < 0.f ||
         directionToTarget.SizeSquared() < m_epsilonSquaredDistanceTolerance))
    {
        directionToTarget = m_finalPath.Pop(false) - actorLocation;
    }

    float directionVectSquaredSize = directionToTarget.SizeSquared();

    if(m_finalPath.Num() == 0 && directionVectSquaredSize < m_epsilonSquaredDistanceTolerance)
    {// Already at the goal
        owner->GetMovementComponent()->Velocity = FVector::ZeroVector;
        return EPathFollowingRequestResult::AlreadyAtGoal;
    }

    if(directionVectSquaredSize > 1.f)
    {
        directionToTarget /= FMath::Sqrt(directionVectSquaredSize);
    }

    owner->GetMovementComponent()->Velocity = directionToTarget * m_droneVelocity;

    return EPathFollowingRequestResult::RequestSuccessful;
}

void ADroneAIController::setDestination(const FVector& newDestinationPosition)
{
    m_destination = newDestinationPosition;
    this->processPath();
}

void ADroneAIController::updateTargetedHeight() USE_NOEXCEPT
{
    switch(m_state)
    {
        case DRONE_RECHARGE:
            m_targetedHeight = m_reloadHeight; //m_targetToFollow->GetActorLocation().Z;
            break;
        default:
            m_targetedHeight = m_destination.Z + m_stationaryElevation;
    }
}

void ADroneAIController::IAUpdate(float deltaTime)
{
    if(!m_actionFinished)
    {
        (this->*m_performAction)();
    }
    else
    {
        // Eval UT and initialise Action
        chooseNextAction();
        m_idleTimer = 0.f;
        switch(m_state)
        {
            case DRONE_WAITING:
                setWaiting();
                break;
            case DRONE_MOVING:
                // TODO - Take king in consideration
                setFollowGroup();
                break;
            case DRONE_COMBAT:
                setFollowFireZone();
                break;
            case DRONE_RECHARGE:
                setFollowSafeZone();
                break;
        }
    }

    if (m_showDestination)
    {
        DrawDebugSphere(
            GetWorld(),
            m_destination,
            24,
            32,
            FColor(0, 0, 255)
        );
    }
}

void ADroneAIController::dropBomb()
{
    ADrone * drone = Cast<ADrone>(this->GetPawn());
    FVector test = drone->GetActorLocation();
    if(drone)
    {
        if(drone->isLoaded())
        {
            PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "BOMB DROOOOOOOOOOOOOOOOOOOOP!!!!!!!!!!!!!!!");
        }
        drone->drop();
    }
}

void ADroneAIController::findDropZone()
{
    //TODO: Improve this a lot, instead of first ennemy!

    // Example: Voronoi Diagram+Fortune's algorithm generation+Delaunay Triangulation / circum-circles  ?
}

void ADroneAIController::followKing()
{
    this->setDestination(
        (m_groupBarycenter * m_alliesAliveCount + m_coeffKing * m_king->GetActorLocation()) / 
        (m_alliesAliveCount + m_coeffKing)
    );
}

void ADroneAIController::followGroup()
{
    // TODO - follow the path
    if(MoveToTarget() == EPathFollowingRequestResult::AlreadyAtGoal)
    {
        m_actionFinished = true;
    }
}


void ADroneAIController::followFireZone()
{
    ADrone* drone = Cast<ADrone>(GetPawn());
    if(MoveToTarget() == EPathFollowingRequestResult::AlreadyAtGoal)
    {
        if(m_canDropBomb)
        {
            dropBomb();
            m_actionFinished = true;
        }
        m_canDropBomb = !m_canDropBomb;
    }
}

void ADroneAIController::followSafeZone()
{
    if(this->HasABomb() || m_idleTimer > m_updateSafeZoneCooldownTime)
    {
        m_actionFinished = true;
        return;
    }
    if(MoveToTarget() == EPathFollowingRequestResult::AlreadyAtGoal)
    {
        m_idleTimer += m_timeSinceLastUpdate;
    }
}

void  ADroneAIController::waiting()
{
    m_actionFinished = true;
    m_idleTimer += m_timeSinceLastUpdate;
}

void ADroneAIController::setFollowGroup()
{
    m_actionFinished = false;
    this->m_performAction = &ADroneAIController::followGroup;

    this->setDestination({ m_groupBarycenter.X, m_groupBarycenter.Y, m_stationaryElevation });
    // TODO - find and set the destination
}

void ADroneAIController::setFollowKing()
{
    if(m_king) //The king is here
    {
        this->m_performAction = &ADroneAIController::followKing;
        m_actionFinished = false;
        // TODO - find and set the destination
    }
    else
    {
        setFollowGroup(); //if no king, stay with group
    }
}

void ADroneAIController::setFollowFireZone()
{
    m_actionFinished = false;
    m_canDropBomb = false;
    this->m_performAction = &ADroneAIController::followFireZone;

    this->setDestination({
        m_bestBombLocation.X,
        m_bestBombLocation.Y,
        m_bestBombLocation.Z + m_stationaryElevation
    });
}

void ADroneAIController::setFollowSafeZone()
{
    m_actionFinished = false;
    this->m_performAction = &ADroneAIController::followSafeZone;

    m_safeZone = findSafeZone();
    setDestination(m_safeZone);
}

void ADroneAIController::setWaiting()
{
    m_actionFinished = false;
    this->m_performAction = &ADroneAIController::waiting;
    // TODO - find and set the destination
}


void ADroneAIController::chooseNextAction()
{
    float scoresArray[DRONE_ACTION_COUNT] = {
        getWaitingScore(),
        getFollowScore(),
        getAttackScore(),
        getReloadScore()
    };

    if(m_currentTime >= m_nextDebugDisplayTime && m_isDebugEnabled)
    {
        m_nextDebugDisplayTime = m_currentTime + 1.5f;
        ADrone * drone = Cast<ADrone>(this->GetPawn());
        drone->displayScore(scoresArray);
    }

    GEngine->AddOnScreenDebugMessage(15, 5.f, FColor::White, "waitScore : " + FString::SanitizeFloat(scoresArray[DRONE_WAITING]));
    GEngine->AddOnScreenDebugMessage(16, 5.f, FColor::White, "followScore : " + FString::SanitizeFloat(scoresArray[DRONE_MOVING]));
    GEngine->AddOnScreenDebugMessage(18, 5.f, FColor::White, "attackScore : " + FString::SanitizeFloat(scoresArray[DRONE_COMBAT]));
    GEngine->AddOnScreenDebugMessage(17, 5.f, FColor::White, "reloadScore : " + FString::SanitizeFloat(scoresArray[DRONE_RECHARGE]));

    float bestScore = -1.f;

    for (int iter = 0; iter < DRONE_ACTION_COUNT; ++iter)
    {
        if (scoresArray[iter] > bestScore)
        {
            bestScore = scoresArray[iter];
            switch (iter)
            {
            case DRONE_MOVING:
                m_state = DRONE_MOVING;
                break;

            case DRONE_COMBAT:
                m_state = DRONE_COMBAT;
                break;

            case DRONE_RECHARGE:
                m_state = DRONE_RECHARGE;
                break;

            case DRONE_WAITING:
            default:
                m_state = DRONE_WAITING;
            }
        }
    }
}

void ADroneAIController::CheckEnnemyNear(float range)
{
    this->CheckEnnemyNearPosition(this->GetPawn()->GetActorLocation(), range);
}

void ADroneAIController::CheckEnnemyNearPosition(const FVector& position, float range)
{
    //TODO: Ray cast instead... Drone currently sees through walls...

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3), // Robots
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4), // Sovec
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6), // Beasts
    };
    TArray<AActor*> ActorsToIgnore{ this->GetPawn() };
    TArray<FHitResult> OutHits;

    m_sensedEnnemies.Reset();

    if(UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetWorld(),
        position,
        position,
        range,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::ForOneFrame,
        OutHits,
        true
    ))
    {
        for(int32 i = 0; i < OutHits.Num(); i++)
        {
            FHitResult& hit = OutHits[i];
            ARobotRebellionCharacter* RRCharacter = Cast<ARobotRebellionCharacter>(hit.GetActor());
            if(NULL != RRCharacter)
            {
                if(RRCharacter->isDead() || !RRCharacter->isVisible())
                {
                    continue;
                }
                m_sensedEnnemies.Add(RRCharacter);
                //break; //DISABLE BREAKS BOMB SEARCH 
            }
        }
    }
}

int ADroneAIController::getNbBombPlayers()
{
    int bombCount = 0;
    int nbPlayers = m_alliesAliveCount;
    for(int noplayer = 0; noplayer < nbPlayers; ++noplayer)
    {
        APlayableCharacter* currentPlayer = Cast<APlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), noplayer));
        bombCount += currentPlayer->getBombCount();
    }
    return bombCount;
}

float ADroneAIController::getBombScore(const FVector& position)
{
    ADrone* owner = Cast<ADrone>(this->GetPawn());

    float playerWillBeKilled = 0.f;
    float numberFriendlyAttacked = 0.f;
    float gameEndIsNear = 0.f; //TODO
    float enemiesAttacked = 0.f;
    float enemiesKilled{};

    //CHECK DAMAGED TARGETS
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2), // Players 
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3), // Robots
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4), // Sovec
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6), // Beasts
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel7), // King
    };
    TArray<AActor*> ActorsToIgnore{ owner };
    TArray<FHitResult> OutHits;

    if(UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetWorld(),
        position,
        position,
        owner->getBombRadius(),
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        OutHits,
        true
    ))
    {
        int32 hitCount = OutHits.Num();
        for(int32 i = 0; i < hitCount; i++)
        {
            FHitResult& hit = OutHits[i];
            ARobotRebellionCharacter* RRCharacter = Cast<ARobotRebellionCharacter>(hit.GetActor());
            if(NULL != RRCharacter)
            {
                if(RRCharacter->isDead() || !RRCharacter->isVisible())
                {
                    continue;
                }

                AKing* king = Cast<AKing>(RRCharacter);
                if(king)
                {
                    return 0.f; // just dont shoot the king
                }

                if(Cast<APlayableCharacter>(RRCharacter)) //is a player
                {
                    numberFriendlyAttacked += 1.f;
                    if(RRCharacter->getHealth() < owner->getBombBaseDamage())
                    {
                        //playerWillBeKilled += 1.f;
                        // TODO - See if the bomb end the fight
                        return 0.f;
                    }
                }
                else
                {
                    if(RRCharacter->getHealth() < owner->getBombBaseDamage())
                    {
                        ++enemiesKilled;
                    }
                    ++enemiesAttacked;
                }
            }
        }
    }

    float score = 0.40f * (enemiesKilled / enemiesAttacked);
    score += 0.35f * (enemiesAttacked / m_sensedEnnemies.Num());
    score += 0.25f * ((4.f - numberFriendlyAttacked) / 4.f);
    //score = (1.f - playerWillBeKilled - gameEndIsNear) * ((1.f / (numberFriendlyAttacked + 1.f) + ennemIsAttacked / getNbEnnemiesInScene()) / c_Normalize);
    
    //float temp = FMath::Clamp((m_sensedEnnemies.Num() - getNbAliveAllies()) / 4.f, 0.f, 1.f);
    float minimalEnemyNumber = 3.f;
    float temp = enemiesAttacked / minimalEnemyNumber;
    temp = temp > 1.f ? 1.f : temp;
    return temp * score; //> temp ? temp : score;
}

FVector ADroneAIController::findSafeZone()
{
    FVector zoneCenter = (5.f * m_groupBarycenter - m_ennemyNearBarycenter) / 4.f;
    zoneCenter.Z = m_reloadHeight;
    return zoneCenter;
}

void ADroneAIController::clearSplinePath()
{
    m_splinePath->ClearSplinePoints(false);
}

void ADroneAIController::updateSplinePath(float tension)
{
    this->clearSplinePath();

    const int32 lastPointIndex = m_smoothedPath.Num() - 1;
    for(int32 iter = 0; iter < lastPointIndex; ++iter)
    {
        m_splinePath->AddSplinePoint(m_smoothedPath[iter], ESplineCoordinateSpace::World, false);
    }

    //update spline at the very end. It is useless to update it before.
    m_splinePath->AddSplinePoint(m_smoothedPath[lastPointIndex], ESplineCoordinateSpace::World, true);

    m_splinePath->GetSplinePointsPosition().AutoSetTangents(tension);
}

void ADroneAIController::smoothPath()
{
    // Smooth the path beginning at the end point
    int32 currentIndex{};
    int32 nextIndex{};

    // Smooth the path beginning at the start point
    m_smoothedPath.Reset();
    currentIndex = m_path.Num() - 1;
    m_smoothedPath.Emplace(m_path[currentIndex]);
    while(currentIndex > 0)
    {
        nextIndex = currentIndex;
        bool canGoTo = true;
        while(canGoTo)
        {
            nextIndex--;
            if(nextIndex == -1)
            {
                canGoTo = false;
            }
            else
            {
                canGoTo = testFlyFromTo(m_path[currentIndex], m_path[nextIndex]);
            }
        }
        ++nextIndex;
        m_smoothedPath.Emplace(m_path[nextIndex]);
        //re -loop from the last index
        currentIndex = nextIndex;
    }

    this->updateSplinePath(m_splineTension);

    this->splineForecast();
}

bool ADroneAIController::testFlyFromTo(const FVector& startPoint, const FVector& endPoint)
{
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{ UEngineTypes::ConvertToObjectType(ECC_WorldStatic) };
    TArray<AActor*> ActorsToIgnore{ GetPawn() };
    TArray<FHitResult> hitActors;
    bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
                                                                   startPoint,
                                                                   endPoint,
                                                                   10.0f,
                                                                   ObjectTypes,
                                                                   false,
                                                                   ActorsToIgnore,
                                                                   EDrawDebugTrace::None,
                                                                   hitActors,
                                                                   true);
    return hitActors.Num() == 0;
}

void ADroneAIController::debugDrawPath() const
{
    //path
    if (m_showOriginPath)
    {
        this->debugElementaryDrawPath(m_path, FColor::Red);
    }

    // smoothed path
    if (m_showSmoothedPath)
    {
        this->debugElementaryDrawPath(m_smoothedPath, FColor::Green);
    }

    // final path
    if(m_showFinalPath)
    {
        this->debugElementaryDrawPath(m_finalPath, FColor::Blue);
    }
}

void ADroneAIController::debugElementaryDrawPath(const TArray<FVector>& pathToDraw, const FColor& lineColor) const
{
    int32 lastTracedPathPoint = pathToDraw.Num() - 1;
    UWorld* world = GetPawn()->GetWorld();

    for(int32 index{}; index < lastTracedPathPoint; ++index)
    {
        DrawDebugLine(world, pathToDraw[index], pathToDraw[index + 1], lineColor, false, 15.f, 0, 5.f);
    }
}

void ADroneAIController::processPath()
{
    NavigationVolumeGraph& myGraph = NavigationVolumeGraph::getInstance();

    int32 currentLocId = myGraph.getOverlappingVolumeId(GetPawn()->GetActorLocation());

    if(currentLocId != -1)
    {
        int32 targetId = myGraph.getOverlappingVolumeId(m_destination);

        if(targetId != -1)
        {
            m_path.Reset();
            m_path.Add(m_destination);
            m_path.Append(myGraph.processAStar(currentLocId, targetId)); // always begin at id 0 node
            m_path.Emplace(GetPawn()->GetActorLocation());
            PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Emerald, "new target id : " + FString::FromInt(targetId));

            smoothPath();
            debugDrawPath();
        }
        else
        {
            float offset = 10.f;
            targetId = myGraph.getBelowVolume(m_destination,
                                              Cast<ADrone>(GetPawn())->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + offset);
            if(targetId != -1)
            {
                m_path.Reset();
                m_path.Add(m_destination);
                m_path.Append(myGraph.processAStar(currentLocId, targetId)); // always begin at id 0 node
                m_path.Emplace(GetPawn()->GetActorLocation());
                PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Emerald, "new target id : " + FString::FromInt(targetId));

                smoothPath();
                debugDrawPath();
            }
        }
    }
}

void ADroneAIController::splineForecast()
{
    const int32 totalPointCount = m_splinePath->GetNumberOfSplinePoints() * m_splinePointCountIntraSegment;
    const float timeStep = m_splinePath->Duration / static_cast<float>(totalPointCount);

    m_finalPath.Reset(totalPointCount);

    float currentTime = m_splinePath->Duration;
    const int32 lastPoint = totalPointCount - 1;

    for(int32 iter = 0; iter < lastPoint; ++iter)
    {
        m_finalPath.Add(m_splinePath->GetLocationAtTime(currentTime, ESplineCoordinateSpace::World));
        currentTime -= timeStep;
    }

    m_finalPath.Add(m_splinePath->GetLocationAtTime(0.f, ESplineCoordinateSpace::World));
}