// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CustomAIControllerBase.h"

#include "Character/RobotRebellionCharacter.h"


void ACustomAIControllerBase::BeginPlay()
{
    m_isInCombat = false;
    m_hitDirection = FVector(0, 0, 0);
}

bool ACustomAIControllerBase::hasALivingTarget() const USE_NOEXCEPT
{
    return this->hasTarget() && !m_targetToFollow->IsActorBeingDestroyed() && !m_targetToFollow->isDead();
}

EPathFollowingRequestResult::Type ACustomAIControllerBase::MoveToTarget()
{
    EPathFollowingRequestResult::Type MoveToActorResult = MoveToActor(Cast<AActor>(m_targetToFollow));
//     if(MoveToActorResult == EPathFollowingRequestResult::Failed && m_hitDirection.Size())
//     {
//         EPathFollowingRequestResult::Type MoveInDirection = MoveToLocation(GetPawn()->GetActorLocation() + m_hitDirection);
//         return MoveInDirection;
//     }

    return MoveToActorResult;
}

void ACustomAIControllerBase::setCombat(bool isCombat, FVector direction /*= FVector(0, 0, 0)*/)
{
    m_isInCombat = isCombat;
    if(isCombat)
    {
        m_hitDirection = direction;
    }
}
