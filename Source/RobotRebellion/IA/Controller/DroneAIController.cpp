// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "DroneAIController.h"
#include "../../Character/King.h"
#include "Character/RobotRebellionCharacter.h"
#include "Character/NonPlayableCharacter.h"
#include "Map.h"
#include "Character/PlayableCharacter.h"

void ADroneAIController::BeginPlay()
{
    Super::BeginPlay();

    m_bestBombLocation = FVector(0, 0, 0);

    m_targetToFollow = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); // for testing

    m_currentTime = 0.f;

    m_nextMovementUpdateTime = m_updateMovementTime;
    m_nextUpdatePropertyTime = m_updatePropertyTime;
    m_nextUpdateAttackCooldownTime = m_updateAttackCooldownTime;

    m_state = DRONE_MOVING; //for testing
    m_coeffKing = 3.f;
    setFollowGroup();
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

float ADroneAIController::getAttackScore()
{
    CheckEnnemyNear(m_detectionDistance);

    float score = 1.0f;
    float bestBombScore = 0.0f;

    TMap<FVector, float> scoreBombLocations;
    if (!m_gotBomb)
    {
        score = 0.f;
    }
    else
    {
////////TODO: Smarter bomb zone calculation, instead of direcly on an ennemy...
        // FOR up to 5 BOMB POSITIONS
        const int iMax = m_sensedEnnemies.Num() < 5 ? m_sensedEnnemies.Num() : 5;
        for(int i = 0; i < iMax; i++)
        {
            //TODO: CALL findDropZone()
            scoreBombLocations.Add(m_sensedEnnemies[i]->GetActorLocation(), getBombScore(m_sensedEnnemies[i]->GetActorLocation()));
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
    if (isInCombat())
    {
        score = 0.f;
    }
    else
    {
        score = 1 - 1 / (0.1f+distance(m_destination)); //Change later
    }
    return score;
}

float ADroneAIController::getReloadScore()
{
    float score = 1.0f;
    // No Bomb
    if (m_gotBomb || (getNbBombPlayers() == 0))  // no reloading possible
    {
        score = 0.0f;
    }
    else
    {
        if (isInCombat())
        {
            score *= (1 - (getNbAliveAllies() / (4 * getNbAliveEnnemies()))); //if not in combat, combat score always =1
        }
        score *= (1 - getNbEnnemiesInZone(FVector(0, 0, 0)) / (0.1f + distance(FVector(0, 0, 0)))); //ZoneScore
    }

    //Is it worth it??? 
        //TBD

    //Safe Zone available
        //Anyone in safe zone

// Bomb
    return score;
}

float ADroneAIController::getWaitingScore()
{
    return 0.f;
}

float ADroneAIController::getDropSCore()
{
    float score = 0.0f;

    ANonPlayableCharacter* owner = Cast<ANonPlayableCharacter>(this->GetPawn());
    FVector dronePosition = owner->GetActorTransform().GetLocation();

    //PRINT_MESSAGE_ON_SCREEN(FColor::White, FString::Printf(TEXT("DROP DISTANCE: %f"), FVector(dronePosition - m_destination).Size()));
    if(FVector(dronePosition - m_destination).Size() < 150.0f && m_gotBomb)
    {
        score = 100.0;
    }
    return score;
}

ADroneAIController::ADroneAIController() : ACustomAIControllerBase()
{
    PrimaryActorTick.bCanEverTick = true;
}


bool ADroneAIController::isInCombat()
{
    return false;
}
int ADroneAIController::getNbAliveAllies()
{
    int nbPlayers = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();
    int res = 0;
    for (int noplayer=0;noplayer<nbPlayers;++noplayer)
    {
        APlayableCharacter* currentPlayer = Cast<APlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), noplayer));
        if (!currentPlayer->isDead())
        {
            ++res;
        }
    }
        return res;
}
int ADroneAIController::getNbAliveEnnemies()
{
    return 0;
}

int ADroneAIController::getNbEnnemiesInZone(FVector zoneCenter)
{
    return 0;
}
float ADroneAIController::distance(FVector dest)
{
    if (this->GetOwner())
    {
        FVector distanceToCompute = dest-this->GetOwner()->GetActorLocation();
        return sqrt(FVector::DotProduct(distanceToCompute,distanceToCompute));
    }
    return 0.f;
}


EPathFollowingRequestResult::Type ADroneAIController::MoveToTarget()
{
    ANonPlayableCharacter* owner = Cast<ANonPlayableCharacter>(this->GetPawn());

    FVector dronePosition = owner->GetActorTransform().GetLocation();
    FVector directionToTarget = m_destination - dronePosition;
    directionToTarget.Z = m_targetedHeight - dronePosition.Z;

    float length = directionToTarget.SizeSquared();

    if (length < 1.f)
    {
        owner->GetMovementComponent()->Velocity = FVector{ 0.f, 0.f, 0.f };

        return EPathFollowingRequestResult::AlreadyAtGoal;
    }

    length = FMath::Sqrt(length);

    if (length > owner->m_moveSpeed)
    {
        directionToTarget /= length;

        owner->GetMovementComponent()->Velocity = directionToTarget * owner->m_moveSpeed;
    }
    else
    {
        owner->GetMovementComponent()->Velocity = directionToTarget * m_decelerationCoefficient;
    }

    return EPathFollowingRequestResult::RequestSuccessful;
}

void ADroneAIController::updateTargetedHeight() USE_NOEXCEPT
{
    m_targetedHeight = m_targetToFollow->GetActorLocation().Z + m_stationaryElevation;
}

void ADroneAIController::updateTargetedTarget()
{
    (this->*m_updateTargetMethod)();
}

void ADroneAIController::IAUpdate(float deltaTime)
{
    if (m_currentTime >= m_nextUpdatePropertyTime)
    {
        updateTargetedTarget();

        updateTargetedHeight();

        m_nextUpdatePropertyTime = m_currentTime + m_updatePropertyTime;
    }

    //if (m_currentTime >= m_updateAttackCooldownTime)
    //{
    //    m_nextUpdateAttackCooldownTime = m_currentTime + m_updateAttackCooldownTime;
    //}
}

void ADroneAIController::IALoop(float deltaTime)
{
    chooseNextAction();

    switch (m_state)
    {
    case DRONE_WAITING:
        break;

    case DRONE_MOVING:
    {
        if (m_currentTime >= m_nextMovementUpdateTime)
        {
            this->MoveToTarget();

            m_nextMovementUpdateTime = m_currentTime + m_updateMovementTime;
        }
        break;
    }

    case DRONE_COMBAT:
        setFollowFireZone();
        if (m_currentTime >= m_nextMovementUpdateTime)
        {
            this->MoveToTarget();

            m_nextMovementUpdateTime = m_currentTime + m_updateMovementTime;
        }

        break;
    case DRONE_BOMB:
        dropBomb();
        break;
    }
}

void ADroneAIController::dropBomb()
{
    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "BOMB DROOOOOOOOOOOOOOOOOOOOP!!!!!!!!!!!!!!!");

    m_gotBomb = false;
}

void ADroneAIController::findDropZone()
{
    //TODO: Improve this a lot, instead of first ennemy!
}

void ADroneAIController::followKing()
{
    int livingPlayers = 0;
    m_destination = FVector(0, 0, 0);
    int32 playerCount = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();
    for (int32 iter = 0; iter < playerCount; ++iter)
    {
        ARobotRebellionCharacter* currentPlayer = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), iter));
        if (currentPlayer) // Protection when player leaves the game
        {
            if (!currentPlayer->isDead())
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
    for (int32 iter = 0; iter < playerCount; ++iter)
    {
        ARobotRebellionCharacter* currentPlayer = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), iter));
        if (currentPlayer) // Protection when player leaves the game
        {
            if (!currentPlayer->isDead())
            {
                m_destination += currentPlayer->GetActorLocation();
                ++livingPlayers;
            }
        }
    }
    if (livingPlayers > 0)
    {
        m_destination /= livingPlayers;
    }
}


void ADroneAIController::followFireZone()
{
    m_destination = m_bestBombLocation;
    m_destination.Z = m_destination.Z + m_stationaryElevation;
}

void ADroneAIController::setFollowGroup()
{
    this->m_updateTargetMethod = &ADroneAIController::followGroup;
}
void ADroneAIController::setFollowKing()
{
    TArray<AActor*> kings;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), m_kingClass, kings);
    if (kings.Num() > 0) //The king is here
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

void ADroneAIController::chooseNextAction()
{
    m_scores.Empty();

    m_scores.Add(DRONE_MOVING, getFollowScore());
    m_scores.Add(DRONE_RECHARGE, getReloadScore());
    m_scores.Add(DRONE_COMBAT, getAttackScore());
    m_scores.Add(DRONE_BOMB, getDropSCore());
    m_scores.ValueSort(&ScoreSortingFunction);
    TArray<AIDroneState> sortedStates;
    m_scores.GenerateKeyArray(sortedStates);
    m_state = sortedStates[0];
}

void ADroneAIController::CheckEnnemyNear(float range)
{
    ANonPlayableCharacter* owner = Cast<ANonPlayableCharacter>(this->GetPawn());
    FVector dronePosition = owner->GetActorTransform().GetLocation();
    FVector MultiSphereStart = dronePosition;
    FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, 15.0f);
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3)); // Robots
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4)); // Sovec
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6)); // Beasts
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(owner);
    //ActorsToIgnore.Add(player); //TODO consider
    TArray<FHitResult> OutHits;
    bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
        MultiSphereStart,
        MultiSphereEnd,
        range, //TODO
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::ForOneFrame,
        OutHits,
        true);

    m_sensedEnnemies.Empty();

    if (Result == true)
    {
        for (int32 i = 0; i < OutHits.Num(); i++)
        {
            FHitResult Hit = OutHits[i];
            ARobotRebellionCharacter* RRCharacter = Cast<ARobotRebellionCharacter>(Hit.GetActor());
            if (NULL != RRCharacter)
            {
                if (RRCharacter->isDead() || !RRCharacter->isVisible())
                {
                    continue;
                }
                m_sensedEnnemies.Add(RRCharacter);
                break; //BUGBUG: ??? 
            }
        }
    }

}

void ADroneAIController::AttackTarget() const
{

}

int ADroneAIController::getNbBombPlayers()
{
    int bombCount = 0;
    int nbPlayers = getNbAliveAllies();
    for (int noplayer=0;noplayer<nbPlayers;++noplayer)
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
    float kingAttacked = 0.f;
    float playerWillBeKilled = 0.f;
    float numberFriendlyAttacked = 0.f;
    float gameEndIsNear = 0.f; //TODO
    float ennemisAttacked = 0.f;

    //CHECK DAMAGED TARGETS
    ANonPlayableCharacter* owner = Cast<ANonPlayableCharacter>(this->GetPawn());
    FVector MultiSphereStart = position;
    FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, c_bombDamageRadius);
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
                                                                   c_bombDamageRadius, //TODO
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
        if(kingAttacked < 0.98f)
        {
            AKing* king = Cast<AKing>(character);
            if(king)
            {
                kingAttacked = 1.f;
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
    score = (1.f - kingAttacked) * (1.f - playerWillBeKilled - gameEndIsNear) * ((1.f / (numberFriendlyAttacked + 1.f) + ennemisAttacked) / c_Normalize);

    return score;
}
