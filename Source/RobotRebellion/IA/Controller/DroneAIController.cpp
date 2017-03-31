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

#define VERY_LITTLE 150.0f

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

    m_bestBombLocation = FVector(0, 0, 0);

    m_targetToFollow = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); // for testing

    m_currentTime = 0.f;
    m_currentAStarTimer = 0.f;

    m_nextMovementUpdateTime = m_updateMovementTime;
    m_nextUpdatePropertyTime = m_updatePropertyTime;
    m_nextUpdateAttackCooldownTime = m_updateAttackCooldownTime;
    m_nextDebugDisplayTime = m_updateAttackCooldownTime;

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

    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, m_destination.ToString());
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
    if(owner)
    {
        FVector dronePosition = owner->GetActorTransform().GetLocation();
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
        if(scoreBombLocations.Num())
        {
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
        score = ( /*BombAvailable+*/(1.f - getNbAliveAllies() / c_NbPlayersMax) + getNbAliveEnnemies()/*/getNbEnnemiesScene()*/ / 15.f + getNbBombPlayers() + bestBombScore) / c_Normalize;
    }
    return score;
}

float ADroneAIController::getFollowScore()
{
    float score;
    if(isInCombat())
    {
        score = 0.f;
    }
    else
    {
        score = 1 - 1 / (0.1f + distance(m_destination)); //Change later
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
        m_safeZone = findSafeZone();
        if(isInCombat())
        {
            //if not in combat, combat score always =1, else it's closer to 1 if many ennemies
            score *= (1 - (getNbAliveAllies() / (4 * getNbAliveEnnemies())));
        }
        score *= (1 - getNbEnnemiesInZone(m_safeZone) / (0.1f + distance(m_safeZone))); //ZoneScore
    }

    //Is it worth it??? 
        //TBD

    //Safe Zone available
        //Anyone in safe zone

// Bomb
    return score;
    //return 1.f;
}

float ADroneAIController::getWaitingScore()
{
    return 0.f;
}

float ADroneAIController::getDropScore()
{
    float score = 0.f;

    ANonPlayableCharacter* owner = Cast<ANonPlayableCharacter>(this->GetPawn());
    FVector dronePosition = owner->GetActorTransform().GetLocation();

    //PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("DROP DISTANCE: %f"), FVector(dronePosition - m_destination).Size()));
    if(FVector(dronePosition - m_destination).Size() < 50.0f && Cast<ADrone>(this->GetPawn())->isLoaded() && m_state == DRONE_COMBAT)
    {
        score = 100.f;
    }
    return score;
}

float ADroneAIController::getNbEnnemiesInScene()
{
    //TODO: Get a way of knowing how many ennemies were spawned originally in the current fight scene.
    return 15.f;
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
    FVector actorLocation = owner->GetActorLocation();

    if(m_smoothedPath.Num() == 0)
    {
        owner->GetMovementComponent()->Velocity = FVector::ZeroVector;
        return EPathFollowingRequestResult::AlreadyAtGoal;
    }

    // Check if we have reach the current point
    if(FVector::Dist(actorLocation, m_smoothedPath.Top()) <= VERY_LITTLE)
    {
        m_smoothedPath.Pop();
    }

    if(m_smoothedPath.Num() == 0)
    {// Already at the goal
        owner->GetMovementComponent()->Velocity = FVector::ZeroVector;
        return EPathFollowingRequestResult::AlreadyAtGoal;
    }
    FVector directionToTarget = m_smoothedPath.Top() - actorLocation;
    directionToTarget.Normalize();

    owner->GetMovementComponent()->Velocity = directionToTarget * 1000.f;

    return EPathFollowingRequestResult::RequestSuccessful;
}

void ADroneAIController::updateTargetedHeight() USE_NOEXCEPT
{
    if(m_targetToFollow)
    {
        switch(m_state)
        {
        case DRONE_RECHARGE:
            m_targetedHeight = m_reloadHeight; //m_targetToFollow->GetActorLocation().Z;
            break;
        default:
            m_targetedHeight = m_targetToFollow->GetActorLocation().Z + m_stationaryElevation;
        }
    }
}

void ADroneAIController::updateTargetedTarget()
{
    (this->*m_updateTargetMethod)();
}

void ADroneAIController::IAUpdate(float deltaTime)
{
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
        //PRINT_MESSAGE_ON_SCREEN(FColor::White, TEXT("DRONE_WAITING"));
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
        //PRINT_MESSAGE_ON_SCREEN(FColor::White, TEXT("DRONE_COMBAT"));
        setFollowFireZone();
        if(m_currentTime >= m_nextMovementUpdateTime)
        {
            this->processPath(deltaTime);
            this->MoveToTarget();

            m_nextMovementUpdateTime = m_currentTime + m_updateMovementTime;
        }
        break;
    case DRONE_BOMB:
        //PRINT_MESSAGE_ON_SCREEN(FColor::White, TEXT("DRONE_BOMB"));
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
    m_destination = FVector(0, 0, 0);
    int32 playerCount = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();
    for(int32 iter = 0; iter < playerCount; ++iter)
    {
        ARobotRebellionCharacter* currentPlayer = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), iter));
        if(currentPlayer) // Protection when player leaves the game
        {
            if(!currentPlayer->isDead())
            {
                m_destination += currentPlayer->GetActorLocation();
                ++livingPlayers;
            }
        }
    }
    m_destination += m_coeffKing * m_king->GetActorLocation();
    m_destination /= (livingPlayers + m_coeffKing);
}

void ADroneAIController::followGroup()
{
    int livingPlayers = 0;
    m_destination = FVector(0, 0, 0);
    int32 playerCount = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();
    for(int32 iter = 0; iter < playerCount; ++iter)
    {
        ARobotRebellionCharacter* currentPlayer = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), iter));
        if(currentPlayer) // Protection when player leaves the game
        {
            if(!currentPlayer->isDead())
            {
                m_destination += currentPlayer->GetActorLocation();
                ++livingPlayers;
            }
        }
    }
    if(livingPlayers > 0)
    {
        m_destination /= livingPlayers;
    }
}


void ADroneAIController::followFireZone()
{
    m_destination = m_bestBombLocation;
    m_destination.Z = m_destination.Z + m_stationaryElevation;
}

void ADroneAIController::followSafeZone()
{
    m_destination = m_safeZone;
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
    //PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("CheckEnnemyNear Range: %f"), range));


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
    float score = 0.f;
    TArray<class ARobotRebellionCharacter *> m_charactersInBlastZone;
    bool kingAttacked = false;
    float playerWillBeKilled = 0.f;
    float numberFriendlyAttacked = 0.f;
    float gameEndIsNear = 0.f; //TODO
    float ennemisAttacked = 0.f;

    //CHECK DAMAGED TARGETS
    ADrone* owner = Cast<ADrone>(this->GetPawn());
    FVector MultiSphereStart = position;
    FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, Cast<ADrone>(GetPawn())->getBombRadius());
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
    if(kingAttacked)
    {
        score = 0.f;
    }
    else
    {
        score = (1.f - playerWillBeKilled - gameEndIsNear) * ((1.f / (numberFriendlyAttacked + 1.f) + ennemisAttacked) / c_Normalize);
    }

    return score;
}

FVector ADroneAIController::findSafeZone()
{
    FVector zoneCenter = FVector(0.f, 0.f, 0.f);
    int actorCount = 0;
    CheckEnnemyNear(zoneCenter, m_safeZoneSize);
    for(int i = 0; i < m_sensedEnnemies.Num(); ++i)
    {
        zoneCenter -= (m_sensedEnnemies[i])->GetActorLocation();
        --actorCount;
    }

    int32 playerCount = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();
    for(int32 iter = 0; iter < playerCount; ++iter)
    {
        ARobotRebellionCharacter* currentPlayer = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), iter));
        if(currentPlayer) // Protection when player leaves the game
        {
            if(!currentPlayer->isDead())
            {
                zoneCenter += currentPlayer->GetActorLocation();
                ++actorCount;
            }
        }
    }

    if(actorCount != 0)
    {
        zoneCenter /= actorCount;
    }
    else
    {
        ARobotRebellionCharacter* currentPlayer = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        zoneCenter += currentPlayer->GetActorLocation();
    }
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

    m_splinePath->AddSplinePoint(GetPawn()->GetActorLocation(), ESplineCoordinateSpace::World, false);

    for(int32 iter = m_smoothedPath.Num() - 1; iter > 0; --iter)
    {
        m_splinePath->AddSplinePoint(m_smoothedPath[iter], ESplineCoordinateSpace::World, false);
    }

    m_splinePath->AddSplinePoint(m_smoothedPath[0], ESplineCoordinateSpace::World, true);

    m_splinePath->GetSplinePointsPosition().AutoSetTangents(tension);

    m_time = 0.f;
}

void ADroneAIController::smoothPath()
{
    // Smooth the path beginning at the end point
    int32 currentIndex{};
    int32 nextIndex{};

    // Smooth the path beginning at the start point
    TArray<FVector> smoothedPath{};
    currentIndex = m_path.Num() - 1;
    smoothedPath.Emplace(m_path[currentIndex]);
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
        smoothedPath.Emplace(m_path[nextIndex]);
        //re -loop from the last index
        currentIndex = nextIndex;
    }

    m_smoothedPath.Reset(smoothedPath.Num());
    m_smoothedPath.AddUninitialized(smoothedPath.Num());
    int32 smoothedPathMaxIndex = smoothedPath.Num() - 1;
    for(int32 index{}; index < smoothedPath.Num(); ++index)
    {
        m_smoothedPath[index] = smoothedPath[smoothedPathMaxIndex - index];
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

void ADroneAIController::debugDrawPath()
{
    //path
    for(int32 index{}; index < m_path.Num() - 1; ++index)
    {
        UWorld* world = GetPawn()->GetWorld();

        DrawDebugLine(world, m_path[index], m_path[index + 1], FColor::Red, false, 15.f, 0, 5.f);
    }
    // smoothed path
    for(int32 index{}; index < m_smoothedPath.Num() - 1; ++index)
    {
        UWorld* world = GetPawn()->GetWorld();

        DrawDebugLine(world, m_smoothedPath[index], m_smoothedPath[index + 1], FColor::Green, false, 15.f, 0, 5.f);
    }
}

void ADroneAIController::processPath(float deltaTime)
{
    m_currentAStarTimer += deltaTime;

    if (m_currentAStarTimer > m_timerAStarProcess)
    {
        NavigationVolumeGraph& myGraph = NavigationVolumeGraph::getInstance();

        int32 currentLocId = myGraph.getOverlappingVolumeId(GetPawn()->GetActorLocation());

        if (currentLocId != -1)
        {
            int32 targetId = myGraph.getOverlappingVolumeId(m_destination);

            if (targetId != -1)
            {
                m_path.Reset();
                m_path.Add(m_destination);
                m_path.Append(myGraph.processAStar(currentLocId, targetId)); // always begin at id 0 node
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

    m_smoothedPath.Reset(totalPointCount);

    float currentTime = m_splinePath->Duration;
    const int32 lastPoint = totalPointCount - 1;

    for(int32 iter = 0; iter < lastPoint; ++iter)
    {
        m_smoothedPath.Add(m_splinePath->GetLocationAtTime(currentTime, ESplineCoordinateSpace::World));
        currentTime -= timeStep;
    }

    m_smoothedPath.Add(m_splinePath->GetLocationAtTime(0.f, ESplineCoordinateSpace::World));
}