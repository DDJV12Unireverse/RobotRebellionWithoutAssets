// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CustomAIControllerBase.h"
#include "RobotRebellionCharacter.h"


EPathFollowingRequestResult::Type ACustomAIControllerBase::MoveToTarget()
{
    EPathFollowingRequestResult::Type MoveToActorResult = MoveToActor(Cast<AActor>(m_targetToFollow));
    return MoveToActorResult;
}

bool ACustomAIControllerBase::hasALivingTarget() const USE_NOEXCEPT
{
    return this->hasTarget() && !m_targetToFollow->IsActorBeingDestroyed() && !m_targetToFollow->isDead();
}