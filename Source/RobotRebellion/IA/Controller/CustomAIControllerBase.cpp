// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CustomAIControllerBase.h"

#include "Character/RobotRebellionCharacter.h"
#include "Global/EntityDataSingleton.h"

ACustomAIControllerBase::ACustomAIControllerBase()
{
    PrimaryActorTick.bCanEverTick = true;
    m_targetToFollow = nullptr;
}

void ACustomAIControllerBase::BeginPlay()
{
    Super::BeginPlay();
    m_showDebugSphereTrace = !!EntityDataSingleton::getInstance().m_showEnnemyDetectionSphere;
}

bool ACustomAIControllerBase::hasALivingTarget() const USE_NOEXCEPT
{
    return this->hasTarget() && !m_targetToFollow->IsActorBeingDestroyed() && !m_targetToFollow->isDead();
}

EPathFollowingRequestResult::Type ACustomAIControllerBase::MoveToTarget()
{
    EPathFollowingRequestResult::Type MoveToActorResult = MoveToActor(Cast<AActor>(m_targetToFollow));

    return MoveToActorResult;
}

bool ACustomAIControllerBase::isInCombat()
{
    return Cast<ARobotRebellionCharacter>(GetPawn())->m_isInCombat;
}

void ACustomAIControllerBase::setTarget(class ARobotRebellionCharacter* attacker)
{
    m_targetToFollow = attacker;

    Cast<ARobotRebellionCharacter>(GetPawn())->m_isInCombat = (attacker != nullptr);
}

void ACustomAIControllerBase::aim(FVector& inOutFireDirection) const USE_NOEXCEPT
{
    inOutFireDirection.Y += FMath::RandRange(-m_aimYMaxFallOffAngle, m_aimYMaxFallOffAngle);
    inOutFireDirection.Z += FMath::RandRange(-m_aimZMaxFallOffAngle, m_aimZMaxFallOffAngle);

    inOutFireDirection.Normalize();
}