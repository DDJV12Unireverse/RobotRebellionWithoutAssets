// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "DroneAIController.h"

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
    FVector directionToTarget = m_targetToFollow->GetActorLocation() - dronePosition;

    directionToTarget.Z = m_targetedHeight - dronePosition.Z;

    float lenght = directionToTarget.SizeSquared();

    if (lenght < 1.f)
    {
        owner->GetMovementComponent()->Velocity = FVector{ 0.f, 0.f, 0.f };

        return EPathFollowingRequestResult::AlreadyAtGoal;
    }

    lenght = FMath::Sqrt(lenght);

    if (lenght > owner->m_moveSpeed)
    {
        directionToTarget /= lenght;

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
    if (this->hasALivingTarget())
    {
        return;
    }

    int32 playerCount = UGameplayStatics::GetGameMode(GetWorld())->GetNumPlayers();

    for (int32 iter = 0; iter < playerCount; ++iter)
    {
        m_targetToFollow = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), iter));

        if (this->hasALivingTarget())
        {
            //good, we've found a new living character to follow...
            break;
        }
    }
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
    }
}
