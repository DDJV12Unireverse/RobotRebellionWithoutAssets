// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "DroneAIController.h"
#include "Character/King.h"
#include "Character/RobotRebellionCharacter.h"
#include "Character/NonPlayableCharacter.h"
#include "Character/PlayableCharacter.h"
#include "Character/Drone.h"
#include "Gameplay/Weapon/Kaboom.h" 
#include "Components/SplineComponent.h"
#include "IA/Navigation/NavigationVolumeGraph.h"


#define VERY_LITTLE 5.0f
DEFINE_LOG_CATEGORY(DroneLog);

ADroneAIController::ADroneAIController() : ACustomAIControllerBase()
{
    PrimaryActorTick.bCanEverTick = true;

    m_splinePath = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
    m_splinePath->DefaultUpVector = {0.f, 0.f, 1.f};
    //m_splinePath->SetSelectedSplineSegmentColor(FLinearColor::Blue);
    // EDITOR MODE ONLY - PACKAGE COMPILE ERROR
    //m_splinePath->ScaleVisualizationWidth = 30.f;
    //m_splinePath->bShouldVisualizeScale = true;
    //m_splinePath->bAllowDiscontinuousSpline = false;

    m_idleTimer = 0.f;
    m_actionFinished = true;

    m_king = nullptr;
}

void ADroneAIController::BeginPlay()
{
    Super::BeginPlay();

    m_bestBombLocation = FVector::ZeroVector;

    m_currentTime = 0.f;
    m_debugCooldownDisplayTime = 1.5f;

    m_state = DRONE_WAITING; //for testing

    if (APawn* drone = GetPawn()) //not so sure who spawn before who. So checking is the best
    {
        m_destination = drone->GetActorLocation();
    }
    else
    {
        m_destination = m_defaultDroneSpawnPositionIfProblem;
    }

    m_coeffKing = 3.f;

    m_deccelPercentPath = 1.f - m_accelPercentPath;
    m_deccelerationCoefficient = (m_accelPercentPath == 0.f) ? 0.001f : 1.f - m_deccelPercentPath;
    m_timeIdleRotationMove = 0.f;

    m_idleTranslationSpeed = FMath::CeilToFloat(m_idleTranslationSpeed) * PI; //Must be a multiple of PI

    this->saveDroneLocalization();

    this->resetTripPoint();
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

            if(FVector::DistSquared(current->GetActorLocation(), ennemyPosition) < squaredDetectRange)
            {
                ++m_ennemyNear;
                m_ennemyNearBarycenter += ennemyPosition;
            }
        }
    }

    if(m_ennemyNear != 0)
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
        if(currentPlayer && !currentPlayer->isDead())
        {
            ++m_alliesAliveCount;
            m_groupBarycenter += currentPlayer->GetActorLocation();
        }
    }

    if(m_alliesAliveCount != 0)
    {
        m_groupBarycenter /= m_alliesAliveCount;
    }
    else
    {
        m_groupBarycenter = GetPawn()->GetActorLocation();
    }
}

void ADroneAIController::saveDroneLocalization()
{
    m_realDroneOrient = GetPawn()->GetActorForwardVector();
}

void ADroneAIController::resetIdleRotationGoal()
{
    m_idleForwardGoal = FMath::VRandCone(m_realDroneOrient, m_idleAngle);
    m_timeIdleRotationMove = 0.f;
}

void ADroneAIController::resetIdleTranslationGoal()
{
    m_idleTranslationDirection = FMath::VRandCone(FVector::UpVector, PI);
}

void ADroneAIController::resetTripPoint()
{
    m_totalTripPoint = 1.f;
    m_currentTripPoint = 1;
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
    score = 1 - 320000.f / (0.1f + (GetPawn()->GetActorLocation() - m_groupBarycenter).SizeSquared()); //Change later
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
    // No Bomb
    if(Cast<ADrone>(this->GetPawn())->isLoaded() || (getNbBombPlayers() == 0))  // no reloading possible
    {
        return 0.0f;
    }
    else
    {
        float score = 1.0f;

        FVector dronePosition = GetPawn()->GetActorLocation();
        m_safeZone = findSafeZone();
        float distSafeZoneToPos = 0.f;
        if(isInCombat())
        {
            //if not in combat, combat score always =1, else it's closer to 1 if many ennemies
            score *= (1.f - (m_alliesAliveCount / (4.f * m_ennemyNear)));
        }
        else
        {
            distSafeZoneToPos = FVector::DistSquaredXY(m_safeZone, dronePosition);
            score *= FVector::DistSquaredXY(m_groupBarycenter, dronePosition)
                / distSafeZoneToPos;
            distSafeZoneToPos = FMath::Sqrt(distSafeZoneToPos);
        }
        score *= (1.f - getNbEnnemiesInZone(m_safeZone) / (0.1f + distSafeZoneToPos)); //ZoneScore

        if (score < 0.f)
        {
            return 0.f;
        }

        return score;
    }
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

EPathFollowingRequestResult::Type ADroneAIController::stopDroneMoves(ADrone* drone)
{
    drone->GetMovementComponent()->Velocity = FVector::ZeroVector;
    FRotator droneRotation = drone->GetActorRotation();
    droneRotation.Pitch = 0.f;
    drone->SetActorRotation(droneRotation);

    this->saveDroneLocalization();

    this->resetTripPoint();

    return EPathFollowingRequestResult::AlreadyAtGoal;
}

EPathFollowingRequestResult::Type ADroneAIController::MoveToTarget()
{
    ADrone* drone = Cast<ADrone>(GetPawn());

    if(m_finalPath.Num() == 0)
    {
        return this->stopDroneMoves(drone);
    }

    FVector actorLocation = drone->GetActorLocation();

    FVector directionToTarget = m_finalPath.Top() - actorLocation;
    FVector& velocity = drone->GetMovementComponent()->Velocity;

    // Check if we have reach the current point
    while(
        m_finalPath.Num() != 0 &&
        (FVector::DotProduct(directionToTarget, velocity) < 0.f ||
         directionToTarget.SizeSquared() < m_epsilonSquaredDistanceTolerance))
    {
        directionToTarget = m_finalPath.Pop(false) - actorLocation;
        ++m_currentTripPoint;
    }

    float directionVectSquaredSize = directionToTarget.SizeSquared();

    if(m_finalPath.Num() == 0 && directionVectSquaredSize < m_epsilonSquaredDistanceTolerance)
    {// Already at the goal
        return this->stopDroneMoves(drone);
    }

    float tripCompletion = this->getTravelCompletionPercentage();

    float speedCoefficient = 
        (tripCompletion < m_accelPercentPath) ? tripCompletion / m_accelPercentPath : 
        (tripCompletion > m_deccelPercentPath) ? (1.f - tripCompletion) / m_deccelerationCoefficient :
        1.f;

    constexpr const float minSpeedCoeff = 0.05f;
    if (speedCoefficient < minSpeedCoeff)
    {
        speedCoefficient = minSpeedCoeff;
    }

    if(directionVectSquaredSize > 1.f)
    {
        directionToTarget /= FMath::Sqrt(directionVectSquaredSize);
    }

    /*PRINT_MESSAGE_ON_SCREEN_UNCHECKED(
        FColor::Yellow, 
        FString::Printf(
            TEXT("Travel Speed : %f  Completion : %f  pathP : %f  curr : %d"), 
            speedCoefficient,
            tripCompletion,
            m_totalTripPoint,
            m_currentTripPoint
        )
    );*/

    velocity = directionToTarget * m_droneVelocity * speedCoefficient;

    FVector velocityDownNormalized = velocity.GetSafeNormal();
    if(FVector::DotProduct(directionToTarget, FVector::UpVector) < 0.5f) // the drone not going up with a angle of more than 45 degree 
    {
        velocityDownNormalized.Z -= speedCoefficient; //to make the drone nose point to down
        velocityDownNormalized.Normalize();

        drone->SetActorRotation(
            FQuat::FastLerp(drone->GetActorForwardVector().ToOrientationQuat(), velocityDownNormalized.ToOrientationQuat(), 0.1f)
        );
    }

    return EPathFollowingRequestResult::RequestSuccessful;
}

void ADroneAIController::internalMakeIdleRotation()
{
    FVector droneForwardVector = GetPawn()->GetActorForwardVector();

    if(!(m_idleForwardGoal - droneForwardVector).IsNearlyZero(0.05f))
    {
        GetPawn()->SetActorRotation(
            FQuat::FastLerp(droneForwardVector.ToOrientationQuat(), m_idleForwardGoal.ToOrientationQuat(), m_idleAngleSpeed)
        );
    }
    else
    {
        this->resetIdleRotationGoal();
    }
}

void ADroneAIController::internalMakeIdleTranslation()
{
    float sinCoeff = m_idleTranslationGain * FMath::Sin(m_idleTimer * m_idleTranslationSpeed);

    APawn* drone = GetPawn();
    FVector currentPosition = drone->GetActorLocation();

    FVector wantedPosition = m_destination + m_idleTranslationDirection * sinCoeff;
    FVector deltaPosition = wantedPosition - currentPosition;

    float dist = deltaPosition.SizeSquared();
    //to avoid jumping teleportation, we straph to destination (dumb asservissement simulation)
    if (dist > 1.f)
    {
        wantedPosition = currentPosition + deltaPosition * m_timeSinceLastUpdate; // pf = pi + v * dt

        //FVector forwardVector = drone->GetActorForwardVector();
        //constexpr const float coeff = 0.01f;
        //deltaPosition.Z -= dist * ((FVector::DotProduct(forwardVector, deltaPosition) >= 0.f) ? coeff : -coeff); //nose point to down
        //deltaPosition.Normalize();

        //drone->SetActorRotation(
        //    FQuat::FastLerp(forwardVector.ToOrientationQuat(), deltaPosition.ToOrientationQuat(), 0.1f)
        //);
    }
    //else we caught up to the idle movement translation
    
    drone->SetActorLocation(wantedPosition);
}

void ADroneAIController::makeIdleMove()
{
    this->internalMakeIdleTranslation();
    this->internalMakeIdleRotation();
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
    if(!m_actionFinished && m_currentTime < m_cooldown)
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

        m_currentTime = 0.f;
    }

#ifdef ENABLE_DRONE_DEBUG_DISPLAY
    if(m_showDestination)
    {
        DrawDebugSphere(
            GetWorld(),
            m_destination,
            24,
            32,
            FColor(0, 0, 255)
        );
    }
#endif //ENABLE_DRONE_DEBUG_DISPLAY
}

void ADroneAIController::dropBomb()
{
    ADrone * drone = Cast<ADrone>(this->GetPawn());
    FVector test = drone->GetActorLocation();
    if(drone)
    {
        // Check if there is still living enemy in the drop zone
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{
            UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3), // Robots
            UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4), // Sovec
            UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6), // Beasts
        };
        TArray<AActor*> ActorsToIgnore{drone};
        TArray<FHitResult> OutHits;


        FVector dropLocation = drone->GetActorLocation();
        dropLocation.Z -= m_stationaryElevation;

        UKismetSystemLibrary::SphereTraceMultiForObjects(
            GetWorld(),
            dropLocation,
            dropLocation,
            drone->getBombRadius(),
            ObjectTypes,
            false,
            ActorsToIgnore,
            EDrawDebugTrace::None,
            OutHits,
            true
        );

        if(OutHits.Num())
        {
            drone->drop();
        }
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
    if(m_idleTimer > m_updateSafeZoneCooldownTime)
    {
        m_actionFinished = true;
        return;
    }
    if(this->HasABomb())// || m_idleTimer > m_updateSafeZoneCooldownTime)
    {
        FVector newHeight = GetPawn()->GetActorLocation();
        newHeight.Z = m_stationaryElevation;
        setDestination(newHeight);
        m_performAction = &ADroneAIController::followGroup;
        return;
    }
    if(MoveToTarget() == EPathFollowingRequestResult::AlreadyAtGoal)
    {
        this->makeIdleMove();
        
        if (m_idleTimer < 0.01f)
        {
            this->resetIdleTranslationGoal();
        }
        m_idleTimer += m_timeSinceLastUpdate;

        m_timeIdleRotationMove += m_timeSinceLastUpdate;
        if (m_timeIdleRotationMove > m_idleRotationResetTime)
        {
            this->resetIdleRotationGoal();
        }
    }
}

void  ADroneAIController::waiting()
{
    this->makeIdleMove();
    //m_actionFinished = true;
    if(m_idleTimer < 0.01f)
    {
        this->resetIdleTranslationGoal();
    }
    m_idleTimer += m_timeSinceLastUpdate;

    m_timeIdleRotationMove += m_timeSinceLastUpdate;
    if(m_timeIdleRotationMove > m_idleRotationResetTime)
    {
        this->resetIdleRotationGoal();
    }
}

void ADroneAIController::setFollowGroup()
{
    m_actionFinished = false;
    this->m_performAction = &ADroneAIController::followGroup;

    this->setDestination({m_groupBarycenter.X, m_groupBarycenter.Y, m_stationaryElevation});
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
    //wait for the drone to deccelerate entirely before waiting if it is currently moving
    if (GetPawn()->GetMovementComponent()->Velocity == FVector::ZeroVector) 
    {
        m_actionFinished = false;
        this->m_performAction = &ADroneAIController::waiting;
        // TODO - find and set the destination
    }
}


void ADroneAIController::chooseNextAction()
{
    float scoresArray[DRONE_ACTION_COUNT] = {
        getWaitingScore(),
        getFollowScore(),
        getAttackScore(),
        getReloadScore()
    };

    if(m_currentTime >= m_debugCooldownDisplayTime && m_isDebugEnabled)
    {
        ADrone * drone = Cast<ADrone>(this->GetPawn());
        drone->displayScore(scoresArray);
    }

#ifdef ENABLE_PRINT_ON_SCREEN
    GEngine->AddOnScreenDebugMessage(15, 5.f, FColor::White, "waitScore : " + FString::SanitizeFloat(scoresArray[DRONE_WAITING]));
    GEngine->AddOnScreenDebugMessage(16, 5.f, FColor::White, "followScore : " + FString::SanitizeFloat(scoresArray[DRONE_MOVING]));
    GEngine->AddOnScreenDebugMessage(18, 5.f, FColor::White, "attackScore : " + FString::SanitizeFloat(scoresArray[DRONE_COMBAT]));
    GEngine->AddOnScreenDebugMessage(17, 5.f, FColor::White, "reloadScore : " + FString::SanitizeFloat(scoresArray[DRONE_RECHARGE]));
#endif //ENABLE_PRINT_ON_SCREEN

    float bestScore = -1.f;

    for(int iter = 0; iter < DRONE_ACTION_COUNT; ++iter)
    {
        if(scoresArray[iter] > bestScore)
        {
            bestScore = scoresArray[iter];
            switch(iter)
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
    TArray<AActor*> ActorsToIgnore{this->GetPawn()};
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
        SPHERECAST_DISPLAY_ONE_FRAME,
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
    float gameEndIsNear = 0.f; // TODO
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
    TArray<AActor*> ActorsToIgnore{owner};
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
    float minimalEnemyNumber = 4.f;
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
    int32 previousIndex{1};
    int32 previousCurrentIndex{1};

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
        if(nextIndex == previousIndex && previousCurrentIndex == currentIndex)
        {
            UE_LOG(DroneLog, Log, TEXT("m_smoothedPath:%d"), m_smoothedPath.Num());
            break;
        }
        previousIndex = nextIndex;
        previousCurrentIndex = currentIndex;
        m_smoothedPath.Emplace(m_path[nextIndex]);
        //re -loop from the last index
        currentIndex = nextIndex;
    }

    this->updateSplinePath(m_splineTension);

    this->splineForecast();
}

bool ADroneAIController::testFlyFromTo(const FVector& startPoint, const FVector& endPoint)
{
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{UEngineTypes::ConvertToObjectType(ECC_WorldStatic)};
    TArray<AActor*> ActorsToIgnore{GetPawn()};
    AKaboom* bomb = Cast<ADrone>(GetPawn())->m_currentBomb;
    if(bomb)
    {
        ActorsToIgnore.Emplace(bomb);
    }
    TArray<FHitResult> hitActors;
    float offset = 5.f;
    float radius = Cast<ADrone>(GetPawn())->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - offset;
    bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
                                                                   startPoint,
                                                                   endPoint,
                                                                   radius,
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
#ifdef ENABLE_DRONE_DEBUG_DISPLAY
    //path
    if(m_showOriginPath)
    {
        this->debugElementaryDrawPath(m_path, FColor::Red);
    }

    // smoothed path
    if(m_showSmoothedPath)
    {
        this->debugElementaryDrawPath(m_smoothedPath, FColor::Green);
    }

    // final path
    if(m_showFinalPath)
    {
        this->debugElementaryDrawPath(m_finalPath, FColor::Blue);
    }
#endif //ENABLE_DRONE_DEBUG_DISPLAY
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

    FVector droneLocation = GetPawn()->GetActorLocation();
    int32 currentLocId = myGraph.getOverlappingVolumeId(droneLocation);
    float offset = Cast<ADrone>(GetPawn())->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + 10.f;
    int32 targetId;

    if(currentLocId == -1)
    {
        currentLocId = myGraph.getBelowVolume(droneLocation, offset);
    }
    if(currentLocId != -1)
    {
        targetId = myGraph.getOverlappingVolumeId(m_destination);

        if(targetId != -1)
        {
            m_path.Reset();
            // test if destination is possible
            if(isLocationFree(m_destination))
            {
                m_path.Add(m_destination);
            }
            m_path.Append(myGraph.processAStar(currentLocId, targetId));
            m_path.Emplace(droneLocation);
            PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Emerald, "new target id : " + FString::FromInt(targetId));

            smoothPath();
            debugDrawPath();
        }
        else
        {
            targetId = myGraph.getBelowVolume(m_destination, offset);
            if(targetId != -1)
            {
                m_path.Reset();
                if(isLocationFree(m_destination))
                {
                    m_path.Add(m_destination);
                }
                m_path.Append(myGraph.processAStar(currentLocId, targetId)); // always begin at id 0 node
                m_path.Emplace(droneLocation);
                PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Emerald, "start id : " + FString::FromInt(currentLocId));
                PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Emerald, "new target id : " + FString::FromInt(targetId));

                smoothPath();
                debugDrawPath();
            }
            else
            {
                // No volume found 

            }
        }
    }
    else
    {
        // agent is out of the navigation volume swtich to steering behaviour
        FVector steeringLocation = droneLocation;
        targetId = myGraph.getNearestVolume(steeringLocation, offset);

        FHitResult hitActors{ForceInit};
        TArray<AActor*> actorsToIgnore{this};
        AKaboom* bomb = Cast<ADrone>(GetPawn())->m_currentBomb;
        if(bomb)
        {
            actorsToIgnore.Emplace(bomb);
        }

        // Cast to check if there is an obstacle between agent and sterring destination
        if(UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),
                                                             droneLocation,
                                                             steeringLocation,
                                                             offset,
                                                             {UEngineTypes::ConvertToObjectType(ECC_WorldStatic)},
                                                             false,
                                                             actorsToIgnore,
                                                             SPHERECAST_DISPLAY_DURATION,
                                                             hitActors,
                                                             true))
        {
            // there is an obstacle -> just teleport the agent to the steering location
            GetPawn()->SetActorLocation(steeringLocation);
        }
        else
        {
            m_path.Reset();
            m_path.Emplace(steeringLocation);
            m_path.Emplace(droneLocation);
            smoothPath();
            //debugDrawPath();
        }

    }
}

bool ADroneAIController::isLocationFree(const FVector& loc)
{
    float offset = 10.f;
    float radius = Cast<ADrone>(GetPawn())->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + offset;

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{UEngineTypes::ConvertToObjectType(ECC_WorldStatic)};
    TArray<AActor*> ActorsToIgnore{GetPawn()};
    AKaboom* bomb = Cast<ADrone>(GetPawn())->m_currentBomb;
    if(bomb)
    {
        ActorsToIgnore.Emplace(bomb);
    }
    TArray<FHitResult> hitActors;
    bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
                                                                   loc,
                                                                   loc,
                                                                   radius,
                                                                   ObjectTypes,
                                                                   false,
                                                                   ActorsToIgnore,
                                                                   EDrawDebugTrace::None,
                                                                   hitActors,
                                                                   true);
    return hitActors.Num() == 0;
}

void ADroneAIController::splineForecast()
{
    const int32 totalPointCount = m_splinePath->GetNumberOfSplinePoints() * m_splinePointCountIntraSegment;
    const float timeStep = m_splinePath->Duration / static_cast<float>(totalPointCount);

    m_finalPath.Reset(totalPointCount);

    float currentTime = m_splinePath->Duration;
    const int32 lastPoint = totalPointCount - 1;

    m_finalPath.Add(m_splinePath->GetLocationAtTime(currentTime, ESplineCoordinateSpace::World));
    currentTime -= timeStep;

    for(int32 iter = 1; iter < lastPoint; ++iter)
    {
        m_finalPath.Add(m_splinePath->GetLocationAtTime(currentTime, ESplineCoordinateSpace::World));
        currentTime -= timeStep;
    }

    m_finalPath.Add(m_splinePath->GetLocationAtTime(0.f, ESplineCoordinateSpace::World));

    if(m_noisyTravelRandom != 0.f)
    {
        int32 noisyPathToModifyCount = m_finalPath.Num() - 1;
        for(int32 iter = 1; iter < noisyPathToModifyCount; ++iter)
        {
            internalNoisyTravelTransfertMethod(m_finalPath[iter], m_finalPath[iter + 1]);
        }
    }

    /*
    m_currentTripPoint / m_totalTripPoint = formerTravelCompletion
    m_currentTripPoint + (m_finalPath.Num() + 1) = m_totalTripPoint
    
    => equation system with 2 unknown => m_currentTripPoint and m_totalTripPoint
    */
    float formerTravelCompletion = this->getTravelCompletionPercentage();
    if(formerTravelCompletion > m_deccelPercentPath)
    {
        //invert to be on the acceleration phase => reacceleration
        formerTravelCompletion = 1.f - formerTravelCompletion;
    }

    float notTravelledCompletion = 1.f - formerTravelCompletion;

    constexpr const float minEpsilon = 0.001f;
    if(notTravelledCompletion < minEpsilon) //security check
    {
        notTravelledCompletion = minEpsilon;
    }

    m_totalTripPoint = static_cast<float>(m_finalPath.Num() + 1) / notTravelledCompletion;

    m_currentTripPoint = static_cast<int32>(m_totalTripPoint * formerTravelCompletion) + 1;

    if(m_currentTripPoint < 1 || m_currentTripPoint > m_totalTripPoint) //something went wrong
    {
        m_currentTripPoint = 1;
        m_totalTripPoint = static_cast<float>(m_finalPath.Num() + 1);
    }
}

void ADroneAIController::internalNoisyTravelTransfertMethod(FVector& inOutPoint, const FVector& nextPoint)
{
    FVector perp1;
    FVector perp2;
    inOutPoint.FindBestAxisVectors(perp1, perp2);

    float minNoisyTravelRandom = -m_noisyTravelRandom;

    perp1 *= FMath::RandRange(minNoisyTravelRandom, m_noisyTravelRandom);
    perp2 *= FMath::RandRange(minNoisyTravelRandom, m_noisyTravelRandom);

    inOutPoint += (perp1 + perp2);
}