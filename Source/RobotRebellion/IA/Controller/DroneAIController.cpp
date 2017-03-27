// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "DroneAIController.h"
#include "../../Character/King.h"
#include "Character/RobotRebellionCharacter.h"
#include "Character/NonPlayableCharacter.h"
#include <algorithm>

float ADroneAIController::getAttackScore()
{
    float score = 0.0f;
    // Fighting stance /*Weapon Fired && */
        //Player(s) fired recently + Ennemies near
            CheckEnnemyNear(3000.0); //TODO: Fix
            if( m_sensedEnnemies.Num())
            {
                score += 0.5;
            }
        //Got Bomb
            if(m_gotBomb)
            {
                score += 0.5;
            }
            //Found Drop Zone && won't kill king, won't kill many allies

            //TODO:


        //Ennemies Alive
            //No

    // No
    return score;
}

float ADroneAIController::getFollowScore()
{

    // Fighting stance
    return 0.0f;
}

float ADroneAIController::getReloadScore()
{
    float score = 0.0f;
    // No Bomb
    if(!m_gotBomb)
    {
        score = 1.0f;
    }

        //Is it worth it??? 
            //TBD

        //Safe Zone available
            //Anyone in safe zone

    // Bomb
    return score;
}

ADroneAIController::ADroneAIController() : ACustomAIControllerBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ADroneAIController::BeginPlay()
{
    Super::BeginPlay();

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

    if(m_currentTime >= m_updateAttackCooldownTime)
    {
        m_nextUpdateAttackCooldownTime = m_currentTime + m_updateAttackCooldownTime;
    }
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
        if(m_currentTime >= m_nextMovementUpdateTime)
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
    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "BombAtt");
    //if(canFire && m_projectileClass != NULL)
    //{
    //    // Retrieve the camera location and rotation
    //    FVector cameraLocation;
    //    FRotator muzzleRotation;
    //    user->GetActorEyesViewPoint(cameraLocation, muzzleRotation);

    //    // m_muzzleOffset is in camera space coordinate => must be transformed to world space coordinate.
    //    const FVector MuzzleLocation = cameraLocation + FTransform(muzzleRotation).TransformVector(m_muzzleOffset);
    //    //muzzleRotation.Pitch += LIFT_OFFSET; // lift the fire a little 
    //    UWorld* const World = user->GetWorld();
    //    if(World)
    //    {
    //        FActorSpawnParameters spawnParams;
    //        spawnParams.Owner = user;
    //        spawnParams.Instigator = user->Instigator;

    //        // spawn a projectile
    //        AProjectile* const projectile = World->SpawnActor<AProjectile>(
    //            m_projectileClass,
    //            MuzzleLocation,
    //            muzzleRotation,
    //            spawnParams
    //            );

    //        if(projectile)
    //        {
    //            projectile->setOwner(user);

    //            PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Purple, "BOMB!");

    //            // Fire
    //            const FVector fireDirection = muzzleRotation.Vector();
    //            projectile->InitVelocity(fireDirection);

    //            if(user->Role == ROLE_Authority)
    //            {
    //                playSound(m_longRangeWeaponFireSound, user);
    //            }

    //            reload();
    //        }
    //    }
    //}
    //else
    //{
    //    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Emerald, "Bomb null");
    //}
    m_gotBomb = false;
}

void ADroneAIController::selectDropZone()
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
            if(currentPlayer) // Protection when player leaves the game
            {
                if (!currentPlayer->isDead())
                {
                    m_destination += currentPlayer->GetActorLocation();
                    ++livingPlayers;
                } 
            }
        }
        m_destination += m_coeffKing * m_king->GetActorLocation();
        m_destination /= (livingPlayers+ m_coeffKing);
}

void ADroneAIController::followGroup()
{
    int livingPlayers = 0;
    m_destination = FVector(0, 0, 0);
    int32 playerCount = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();
    for (int32 iter = 0; iter < playerCount; ++iter)
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
    if (livingPlayers > 0)
    {
        m_destination /= livingPlayers;
    }
}


void ADroneAIController::followFireZone()
{

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
    else setFollowGroup(); //if no king, stay with group
}

void ADroneAIController::setFollowFireZone()
{
    this->m_updateTargetMethod = &ADroneAIController::followFireZone;
}

static bool ScoreSortingFunction(const ActionScore& left, const ActionScore& right)
{
    return left.second > right.second;
}

void ADroneAIController::chooseNextAction()
{
    m_scores.clear();
    m_scores.push_back(std::make_pair<AIDroneState, float>(DRONE_COMBAT, getAttackScore()));
    m_scores.push_back(std::make_pair<AIDroneState, float>(DRONE_MOVING, getFollowScore()));
    m_scores.push_back(std::make_pair<AIDroneState, float>(DRONE_MOVING, getReloadScore()));

    std::sort(m_scores.begin(), m_scores.end(), &ScoreSortingFunction);

    m_state = m_scores[0].first;
}

void ADroneAIController::CheckEnnemyNear(float range)
{

    //TODO: Improve this a lot, instead of first ennemy!

    ANonPlayableCharacter* owner = Cast<ANonPlayableCharacter>(this->GetPawn());
    FVector dronePosition = owner->GetActorTransform().GetLocation();
    FVector MultiSphereStart = dronePosition;
    FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, 15.0f);
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    //ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)); // Players  //TODO consider avoiding players
    //ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1)); // Projectile  //TODO consider 
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

    //m_targetToFollow = NULL;
    m_sensedEnnemies.Empty();

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
                //m_targetToFollow = RRCharacter;
                m_destination = RRCharacter->GetActorLocation(); //TODO: Fix me.
                m_sensedEnnemies.Add(RRCharacter);
                break;
            }
        }
    }

}

void ADroneAIController::AttackTarget() const
{

}
