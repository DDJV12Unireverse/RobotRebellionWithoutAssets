// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CustomAIControllerBase.generated.h"

/**
 *
 */
UCLASS()
class ROBOTREBELLION_API ACustomAIControllerBase : public AAIController
{
    GENERATED_BODY()

protected:
    class ARobotRebellionCharacter *m_targetToFollow;


public:
    FORCEINLINE bool hasTarget() const USE_NOEXCEPT
    {
        return m_targetToFollow != NULL;
    }

    ARobotRebellionCharacter* getTarget() const USE_NOEXCEPT
    {
        return m_targetToFollow;
    }

    bool hasALivingTarget() const USE_NOEXCEPT;

    /*
     * VIRTUAL METHODS
     */
    virtual EPathFollowingRequestResult::Type MoveToTarget();

    virtual void CheckEnnemyNear(float range) PURE_VIRTUAL(ACustomAIControllerBase::CheckEnnemyNear, );

    virtual void CheckEnnemyNear(FVector position, float range) PURE_VIRTUAL(ACustomAIControllerBase::CheckEnnemyNear, );

    virtual void AttackTarget() const PURE_VIRTUAL(ACustomAIControllerBase::AttackTarget, );
};
