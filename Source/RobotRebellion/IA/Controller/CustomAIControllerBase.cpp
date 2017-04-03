// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CustomAIControllerBase.h"

#include "Character/RobotRebellionCharacter.h"
#include "Global/EntityDataSingleton.h"

ACustomAIControllerBase::ACustomAIControllerBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACustomAIControllerBase::BeginPlay()
{
    Super::BeginPlay();
    m_isInCombat = false;
    m_hitDirection = FVector(0, 0, 0);
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

void ACustomAIControllerBase::setCombat(bool isCombat, ARobotRebellionCharacter* attacker)
{
    m_isInCombat = isCombat;
    if(isCombat)
    {
        if(!hasALivingTarget())
        {
            m_targetToFollow = attacker;
        }
    }
}
