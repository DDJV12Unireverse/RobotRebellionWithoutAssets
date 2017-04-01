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
    m_splinePath->DefaultUpVector = { 0.f, 0.f, 1.f };
    m_splinePath->ScaleVisualizationWidth = 30.f;
    m_splinePath->SetSelectedSplineSegmentColor(FLinearColor::Blue);
    m_splinePath->bShouldVisualizeScale = true;
    m_splinePath->bAllowDiscontinuousSpline = false;
}

void ADroneAIController::BeginPlay()
{
    Super::BeginPlay();

    m_bestBombLocation = FVector::ZeroVector;

    m_currentTime = 0.f;
    m_currentAStarTimer = 0.f;

    m_nextMovementUpdateTime = m_updateMovementTime;
    m_nextUpdatePropertyTime = m_updatePropertyTime;
    m_nextUpdateAttackCooldownTime = m_updateAttackCooldownTime;
    m_nextDebugDisplayTime = m_updateAttackCooldownTime;

    TArray<AActor*> ennemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANonPlayableCharacter::StaticClass(), ennemies);
    m_ennemisInScene = ennemies.Num() - 2; //do not count drone and king

    m_state = DRONE_MOVING; //for testing
    m_coeffKing = 3.f;
    setFollowGroup();
    //m_gotBomb = true;
}

void ADroneAIController::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    m_currentTime += deltaTime;

    IAUpdate(deltaTime);
    IALoop(deltaTime);
}


static bool ScoreSortingFunction(const float& left, const float& right)
{
    return left > right;
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
    ANonPlayableCharacter* owner = Cast<ANonPlayableCharacter>(this->GetPawn());
    FVector dronePosition;
    if(owner)
    {
        dronePosition = owner->GetActorTransform().GetLocation();
        CheckEnnemyNear(dronePosition, m_detectionDistance);
    }

    float score = 1.0f;
    float bestBombScore = 0.0f;

    TMap<FVector, float> scoreBombLocations;
    if(!Cast<ADrone>(this->GetPawn())->isLoaded())
    {
        score = 0.f;
    }
    else
    {
        //Up to 5 BOMB POSITIONS
        const int iMax = m_sensedEnnemies.Num() < 5 ? m_sensedEnnemies.Num() : 5;
        for(int i = 0; i < iMax; i++)
        {
            //TODO: Implement and call findDropZone() instead
            FVector ennemyPos = m_sensedEnnemies[i]->GetActorLocation();
            float bombScore = getBombScore(ennemyPos);
            scoreBombLocations.Add(ennemyPos, bombScore);
            //   PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("POSITION:%f %f %f BOMB Score: %f"), ennemyPos.X, ennemyPos.Y, ennemyPos.Z, bombScore));
        }

        if(scoreBombLocations.Num() && m_currentTime >= m_nextChangeTargetTime)
        {
            m_nextChangeTargetTime = m_currentTime + m_updateAttackCooldownTime;
            scoreBombLocations.ValueSort(&ScoreSortingFunction);
            TArray<FVector> sortedLocations;
            scoreBombLocations.GenerateKeyArray(sortedLocations);

            m_bestBombLocation = sortedLocations[0];
            bestBombScore = scoreBombLocations[sortedLocations[0]];
        }

        // Use the Best Bomb Score

        const float c_Normalize = 4.f; // 4 additions
        const float c_NbPlayersMax = 4.f; // 4 elements

////////TODO: getNbEnnemiesScene() HARDCODED 15.f for now 

//   PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("POSITION:%f %f %f BOMB Score: %f"), ennemyPos.X, ennemyPos.Y, ennemyPos.Z, bombScore));

        PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("Player alive ratio:%.3f \n EnnemyRatio:%f \n nbBmbPlayr:%d \n bestBombScore:%f \n"), (1.f - getNbAliveAllies() / c_NbPlayersMax), (getNbAliveEnnemies() / getNbEnnemiesInScene()), getNbBombPlayers(), bestBombScore));

        score = ((1.f - getNbAliveAllies() / c_NbPlayersMax) + (getNbAliveEnnemies() / getNbEnnemiesInScene()) + isInCombat() * getNbBombPlayers() + bestBombScore) / c_Normalize;

        score *= m_attackTuningFactor;

        if(FVector(dronePosition - m_destination).Size() < 50.0f && Cast<ADrone>(this->GetPawn())->isLoaded() && m_state == DRONE_COMBAT)
        {
            score *= 0.01f;
        }
    }
    return score;
}

float ADroneAIController::getFollowScore()
{
    float score;
    if(!isInCombat())
    {
        score = 1 - 1.f / (0.1f + distance(m_destination)); //Change later
    }
    else
    {
        score = 1.f - getBombScore(m_bestBombLocation);
    }
    score = (score < 0.f) ? 0.f : score;
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
        m_safeZone = findSafeZone();
        if(isInCombat())
        {
            //if not in combat, combat score always =1, else it's closer to 1 if many ennemies
            score *= (1 - (getNbAliveAllies() / (4 * getNbAliveEnnemies())));
        }
        score *= (1 - getNbEnnemiesInZone(m_safeZone) / (0.1f + distance(m_safeZone))); //ZoneScore
        score = (score < 0.f) ? 0.f : score;
    }

    return score;
}

float ADroneAIController::getWaitingScore()
{
    ADrone* drone = Cast<ADrone>(GetPawn());
    if(isInCombat())
    {
        if((m_destination - drone->GetActorLocation()).Size() < 10.f)
        {
            return 1.f - getBombScore(m_bestBombLocation);
        }
        return 0.f;
    }
    else
    {
        if((m_destination - drone->GetActorLocation()).Size() < 10.f)
        {
            return 1.f;
        }
        return 0.f;
    }
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

float ADroneAIController::getNbEnnemiesInScene()
{
    //TODO: Get a way of knowing how many ennemies were spawned originally in the current fight scene.
    return m_ennemisInScene;
}


bool ADroneAIController::isInCombat()
{
    return (getNbAliveEnnemies() > 0);
}
int ADroneAIController::getNbAliveAllies()
{
    int nbPlayers = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();
    int res = 0;
    for(int noplayer = 0; noplayer < nbPlayers; ++noplayer)
    {
        APlayableCharacter* currentPlayer = Cast<APlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), noplayer));
        if(!currentPlayer->isDead())
        {
            ++res;
        }
    }
    return res;
}
int ADroneAIController::getNbAliveEnnemies()
{
    ANonPlayableCharacter* owner = Cast<ANonPlayableCharacter>(this->GetPawn());
    if(owner)
    {
        FVector dronePosition = owner->GetActorTransform().GetLocation();
        CheckEnnemyNear(dronePosition, m_detectionDistance);
        return (m_sensedEnnemies.Num());
    }
    return 0;
}

int ADroneAIController::getNbEnnemiesInZone(FVector zoneCenter)
{
    CheckEnnemyNear(zoneCenter, m_detectionDistance);
    return (m_sensedEnnemies.Num());
}
float ADroneAIController::distance(FVector dest)
{
    ANonPlayableCharacter* owner = Cast<ANonPlayableCharacter>(this->GetPawn());
    if(owner)
    {
        FVector dronePosition = owner->GetActorLocation();
        FVector distanceToCompute = dest - dronePosition;
        return sqrt(FVector::DotProduct(distanceToCompute, distanceToCompute));
    }
    return 0.f;
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

    FVector globalDirection = m_destination - actorLocation;
    FVector directionToTarget = m_finalPath.Top() - actorLocation;

    // Check if we have reach the current point
    while(
        m_finalPath.Num() != 0 &&
        (FVector::DotProduct(directionToTarget, globalDirection) < 0.f ||
            directionToTarget.SizeSquared() < m_epsilonSquaredDistanceTolerance))
    {
        globalDirection = m_destination - actorLocation;
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
    //this->processPath();
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

void ADroneAIController::updateTargetedTarget()
{
    (this->*m_updateTargetMethod)();
}

void ADroneAIController::IAUpdate(float deltaTime)
{
    m_timeSinceLastUpdate = deltaTime;

    if(m_currentTime >= m_nextUpdatePropertyTime)
    {
        updateTargetedTarget();

        updateTargetedHeight();

        m_nextUpdatePropertyTime = m_currentTime + m_updatePropertyTime;
    }
}

void ADroneAIController::IALoop(float deltaTime)
{
    chooseNextAction();

    switch(m_state)
    {
    case DRONE_WAITING:
        PRINT_MESSAGE_ON_SCREEN(FColor::White, TEXT("DRONE_WAITING"));
        setFollowGroup();
        break;
    case DRONE_MOVING:
    {
        //PRINT_MESSAGE_ON_SCREEN(FColor::White, TEXT("DRONE_MOVING"));
        if(m_currentTime >= m_nextMovementUpdateTime)
        {
            this->processPath(deltaTime);
            this->MoveToTarget();

            m_nextMovementUpdateTime = m_currentTime + m_updateMovementTime;
        }
        break;
    }

    case DRONE_COMBAT:
        setFollowFireZone();
        if(m_currentTime >= m_nextMovementUpdateTime)
        {
            this->processPath(deltaTime);
            this->MoveToTarget();

            m_nextMovementUpdateTime = m_currentTime + m_updateMovementTime;
        }
        break;
    case DRONE_BOMB:
        dropBomb();
        break;
    case DRONE_RECHARGE:
        setFollowSafeZone();
        //PRINT_MESSAGE_ON_SCREEN(FColor::White, TEXT("DRONE_RECHARGE"));
        if(m_currentTime >= m_nextMovementUpdateTime)
        {
            this->processPath(deltaTime);
            this->MoveToTarget();

            m_nextMovementUpdateTime = m_currentTime + m_updateMovementTime;
        }
    }
    DrawDebugSphere(
        GetWorld(),
        m_destination,
        24,
        32,
        FColor(0, 0, 255)
    );
}

void ADroneAIController::dropBomb()
{
    ADrone * drone = Cast<ADrone>(this->GetPawn());
    if(drone)
    {
        if(drone->isLoaded())
        {
            PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "BOMB DROOOOOOOOOOOOOOOOOOOOP!!!!!!!!!!!!!!!");
        }
        drone->GetMovementComponent()->Velocity = FVector::ZeroVector;
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
    int livingPlayers = 0;

    FVector preDestination{ m_coeffKing * m_king->GetActorLocation() };
    int32 playerCount = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();

    for(int32 iter = 0; iter < playerCount; ++iter)
    {
        ARobotRebellionCharacter* currentPlayer = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), iter));
        if(currentPlayer) // Protection when player leaves the game
        {
            if(!currentPlayer->isDead())
            {
                preDestination += currentPlayer->GetActorLocation();
                ++livingPlayers;
            }
        }
    }

    preDestination /= (livingPlayers + m_coeffKing);

    this->setDestination(preDestination);
}

void ADroneAIController::followGroup()
{
    int livingPlayers = 0;

    FVector preDestination = FVector::ZeroVector;
    int32 playerCount = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();

    for(int32 iter = 0; iter < playerCount; ++iter)
    {
        ARobotRebellionCharacter* currentPlayer = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), iter));
        if(currentPlayer) // Protection when player leaves the game
        {
            if(!currentPlayer->isDead())
            {
                preDestination += currentPlayer->GetActorLocation();
                ++livingPlayers;
            }
        }
    }
    
    if(livingPlayers > 0)
    {
        preDestination /= livingPlayers;
    }
    
    preDestination.Z = m_stationaryElevation;

    this->setDestination(preDestination);
}


void ADroneAIController::followFireZone()
{
    this->setDestination({ m_bestBombLocation.X, m_bestBombLocation.Y, m_bestBombLocation.Z + m_stationaryElevation });
}

void ADroneAIController::followSafeZone()
{
    this->setDestination(m_safeZone);
}

void ADroneAIController::setFollowGroup()
{
    this->m_updateTargetMethod = &ADroneAIController::followGroup;
}
void ADroneAIController::setFollowKing()
{
    TArray<AActor*> kings;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), m_kingClass, kings);
    if(kings.Num() > 0) //The king is here
    {
        m_king = Cast<AKing>(kings.Top());
        this->m_updateTargetMethod = &ADroneAIController::followKing;
    }
    else
    {
        setFollowGroup(); //if no king, stay with group
    }
}

void ADroneAIController::setFollowFireZone()
{
    this->m_updateTargetMethod = &ADroneAIController::followFireZone;
}

void ADroneAIController::setFollowSafeZone()
{
    this->m_updateTargetMethod = &ADroneAIController::followSafeZone;
}

void ADroneAIController::chooseNextAction()
{

    float followScore = getFollowScore();
    float reloadScore = getReloadScore();
    float attackScore = getAttackScore();
    float dropScore = getDropScore();
    float waitScore = getWaitingScore();

    if(m_currentTime >= m_nextDebugDisplayTime && m_isDebugEnabled)
    {
        float scoresArray[5] = { followScore, reloadScore, attackScore, dropScore, waitScore };
        m_nextDebugDisplayTime = m_currentTime + 1.5f;
        ADrone * drone = Cast<ADrone>(this->GetPawn());
        drone->displayScore(scoresArray);
    }

    if(m_currentTime >= m_nextUpdateAttackCooldownTime)
    {
        m_nextUpdateAttackCooldownTime = m_currentTime + m_updateAttackCooldownTime;

        m_scores.Reset();
        m_scores.Add(DRONE_MOVING, followScore);
        //PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("DRONE_MOVING Score: %f"), followScore));
        m_scores.Add(DRONE_RECHARGE, reloadScore);
        //PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("DRONE_RECHARGE Score: %f"), reloadScore));
        m_scores.Add(DRONE_COMBAT, attackScore);
        //PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("DRONE_COMBAT Score: %f"), attackScore));
        m_scores.Add(DRONE_BOMB, dropScore);
        //PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("DRONE_BOMB Score: %f"), dropScore));
        m_scores.Add(DRONE_WAITING, waitScore);
        //PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("DRONE_WAIT Score: %f"), waitScore));

        m_scores.ValueSort(&ScoreSortingFunction);

        TArray<AIDroneState> sortedStates;
        m_scores.GenerateKeyArray(sortedStates);
        m_state = sortedStates[0];
    }
}

void ADroneAIController::CheckEnnemyNear(FVector position, float range)
{
    //TODO: Ray cast instead... Drone currently sees through walls...

    ANonPlayableCharacter* owner = Cast<ANonPlayableCharacter>(this->GetPawn());
    FVector dronePosition = owner->GetActorTransform().GetLocation();
    FVector MultiSphereStart = position;
    FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, 15.0f);
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3)); // Robots
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4)); // Sovec
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6)); // Beasts
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(owner);
    TArray<FHitResult> OutHits;
    bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
        MultiSphereStart,
        MultiSphereEnd,
        range,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::ForOneFrame,
        OutHits,
        true);

    m_sensedEnnemies.Reset();

    if(Result == true)
    {
        for(int32 i = 0; i < OutHits.Num(); i++)
        {
            FHitResult Hit = OutHits[i];
            ARobotRebellionCharacter* RRCharacter = Cast<ARobotRebellionCharacter>(Hit.GetActor());
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
    int nbPlayers = getNbAliveAllies();
    for(int noplayer = 0; noplayer < nbPlayers; ++noplayer)
    {
        APlayableCharacter* currentPlayer = Cast<APlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), noplayer));
        bombCount += currentPlayer->getBombCount();
    }
    return bombCount;
}

float ADroneAIController::getBombScore(FVector position)
{
    ADrone* owner = Cast<ADrone>(this->GetPawn());
    float score = 0.f;

    if(owner->isLoaded())
    {
        TArray<class ARobotRebellionCharacter *> m_charactersInBlastZone;
        bool kingAttacked = false;
        float playerWillBeKilled = 0.f;
        float numberFriendlyAttacked = 0.f;
        float gameEndIsNear = 0.f; //TODO
        float ennemisAttacked = 0.f;

        //CHECK DAMAGED TARGETS
        FVector MultiSphereStart = position;
        FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, owner->getBombRadius());
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)); // Players  //TODO consider avoiding players and king
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3)); // Robots
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4)); // Sovec
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6)); // Beasts
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel7)); // King
        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(owner);
        TArray<FHitResult> OutHits;
        bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
            MultiSphereStart,
            MultiSphereEnd,
            owner->getBombRadius(), //TODO
            ObjectTypes,
            false,
            ActorsToIgnore,
            EDrawDebugTrace::ForOneFrame,
            OutHits,
            true);
        //m_targetToFollow = NULL;
        m_charactersInBlastZone.Empty();
        if(Result == true)
        {
            for(int32 i = 0; i < OutHits.Num(); i++)
            {
                FHitResult Hit = OutHits[i];
                ARobotRebellionCharacter* RRCharacter = Cast<ARobotRebellionCharacter>(Hit.GetActor());
                if(NULL != RRCharacter)
                {
                    if(RRCharacter->isDead() || !RRCharacter->isVisible())
                    {
                        continue;
                    }
                    m_charactersInBlastZone.Add(RRCharacter);
                }
            }
        }

        for(auto character : m_charactersInBlastZone)
        {
            if(!kingAttacked)
            {
                AKing* king = Cast<AKing>(character);
                if(king)
                {
                    kingAttacked = true;
                    continue;
                }
            }
            APlayableCharacter* player = Cast<APlayableCharacter>(character);
            if(player)
            {
                numberFriendlyAttacked += 1.f;
                if((player->getHealth() / player->getMaxHealth()) < 0.2f)
                {
                    playerWillBeKilled = 1.0;
                }
            }
            else
            {
                ennemisAttacked += 1.0f;
            }
        }

        const float c_Normalize = 2.f; // 2 additions

        if(kingAttacked || numberFriendlyAttacked > 0)
        {
            score = 0.f;
        }
        else
        {
            score = (1.f - playerWillBeKilled - gameEndIsNear) * ((1.f / (numberFriendlyAttacked + 1.f) + ennemisAttacked / getNbEnnemiesInScene()) / c_Normalize);
        }
    }

    return score;
}

FVector ADroneAIController::findSafeZone()
{
    if(m_currentTime >= m_nextUpdateSafeZoneTime)
    {
        m_nextUpdateSafeZoneTime = m_currentTime + m_updateSafeZoneCooldownTime;
        FVector zoneCenter = FVector(0.f, 0.f, 0.f);
        FVector zoneCenterAllies = FVector(0.f, 0.f, 0.f);
        FVector zoneCenterEnnemies = FVector(0.f, 0.f, 0.f);
        int actorCount = 0;
        CheckEnnemyNear(zoneCenter, m_safeZoneSize);
        for(int i = 0; i < m_sensedEnnemies.Num(); ++i)
        {
            zoneCenterEnnemies += (m_sensedEnnemies[i])->GetActorLocation();
            ++actorCount;
        }
        if(actorCount > 0)
        {
            zoneCenterEnnemies /= actorCount;
        }
        actorCount = 0;

        int32 playerCount = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();
        for(int32 iter = 0; iter < playerCount; ++iter)
        {
            ARobotRebellionCharacter* currentPlayer = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), iter));
            if(currentPlayer) // Protection when player leaves the game
            {
                if(!currentPlayer->isDead())
                {
                    zoneCenterAllies += currentPlayer->GetActorLocation();
                    ++actorCount;
                }
            }
            if(actorCount > 0)
            {
                zoneCenterAllies /= actorCount;
            }
        }

        zoneCenter = (5 * zoneCenterAllies - zoneCenterEnnemies) / 4;
        zoneCenter.Z = m_reloadHeight;
        return zoneCenter;
    }
    else
    {
        return m_safeZone;
    }
}

#define TO_ERASE_AFTER
void ADroneAIController::clearSplinePath()
{
    m_splinePath->ClearSplinePoints(false);
}

void ADroneAIController::updateSplinePath(float tension)
{
    this->clearSplinePath();

#ifndef TO_ERASE_AFTER
    m_splinePath->AddSplinePoint(GetPawn()->GetActorLocation(), ESplineCoordinateSpace::World, false);
#endif

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
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetPawn());
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
    this->debugElementaryDrawPath(m_path, FColor::Red);

    // smoothed path
    this->debugElementaryDrawPath(m_smoothedPath, FColor::Green);

    // final path
    this->debugElementaryDrawPath(m_finalPath, FColor::Blue);
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

void ADroneAIController::processPath(float deltaTime)
{
    m_currentAStarTimer += deltaTime;

    if(m_currentAStarTimer > m_timerAStarProcess)
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
#ifdef TO_ERASE_AFTER
                m_path.Emplace(GetPawn()->GetActorLocation());
#endif
                PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Emerald, "new target id : " + FString::FromInt(targetId));

                smoothPath();
                if(m_showDebugPath)
                {
                    debugDrawPath();
                }
            }
        }
        m_currentAStarTimer = 0.f;
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