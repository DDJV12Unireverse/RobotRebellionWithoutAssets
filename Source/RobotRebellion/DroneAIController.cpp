// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "DroneAIController.h"
#include "King.h"
#include "RobotRebellionCharacter.h"
#include "NonPlayableCharacter.h"

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

    m_state = DRONE_MOVING; //for testing

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
    (this->*m_updateTarget)();
}

void ADroneAIController::IAUpdate(float deltaTime)
{
    if (m_currentTime >= m_nextUpdatePropertyTime)
    {
        updateTargetedTarget();

        updateTargetedHeight();

        m_nextUpdatePropertyTime = m_currentTime + m_updatePropertyTime;
    }
}

void ADroneAIController::IALoop(float deltaTime)
{
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
        break;
    }
}

void ADroneAIController::followKing()
{
        int livingPlayers = 0;
        m_destination = FVector(0, 0, 0);
        int32 playerCount = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();
        for (int32 iter = 0; iter < playerCount; ++iter)
        {
            ARobotRebellionCharacter* currentPlayer = static_cast<ARobotRebellionCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), iter));
            if (!currentPlayer->isDead())
            {
                m_destination += currentPlayer->GetActorLocation();
                ++livingPlayers;
            }
        }
        m_destination += 3 * m_king->GetActorLocation();
        m_destination /= (livingPlayers+3);
}

void ADroneAIController::followGroup()
{
    int livingPlayers = 0;
    m_destination = FVector(0, 0, 0);
    int32 playerCount = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();
    for (int32 iter = 0; iter < playerCount; ++iter)
    {
        ARobotRebellionCharacter* currentPlayer = static_cast<ARobotRebellionCharacter*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), iter));
        if (!currentPlayer->isDead())
        {
            m_destination += currentPlayer->GetActorLocation();
            ++livingPlayers;
        }
    }
    if (livingPlayers > 0)
    {
        m_destination /= livingPlayers;
    }
}


void ADroneAIController::setFollowGroup()
{
    this->m_updateTarget = &ADroneAIController::followGroup;
}
void ADroneAIController::setFollowKing()
{
    TArray<AActor*> kings;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), m_kingClass, kings);
    if (kings.Num() > 0) //The king is here
    {
        m_king = static_cast<AKing*>(kings.Top());
        this->m_updateTarget = &ADroneAIController::followKing;
    }
    else setFollowGroup(); //if no king, stay with group
}