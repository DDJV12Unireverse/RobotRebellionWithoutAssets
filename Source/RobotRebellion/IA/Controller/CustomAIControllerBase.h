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


private:
    class ARobotRebellionCharacter* m_targetToFollow;


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
        bool m_showDebugSphereTrace;


public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = 0.f))
        float m_aimYMaxFallOffAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = 0.f))
        float m_aimZMaxFallOffAngle;

protected:
    FVector getTargetToFollowLocation() const;

public:
    ACustomAIControllerBase();
    virtual ~ACustomAIControllerBase() = default;
	
    virtual void BeginPlay() override;

    FORCEINLINE bool hasTarget() const USE_NOEXCEPT
    {
        return m_targetToFollow != NULL;
    }

    FORCEINLINE ARobotRebellionCharacter* getTarget() const USE_NOEXCEPT
    {
        return m_targetToFollow;
    }

    bool isInCombat();

    void setTarget(class ARobotRebellionCharacter* attacker);

    bool hasALivingTarget() const USE_NOEXCEPT;

    FORCEINLINE EDrawDebugTrace::Type debugDrawTraceShowingMode() const USE_NOEXCEPT
    {
        return m_showDebugSphereTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
    }

    /*
     * VIRTUAL METHODS
     */
    virtual EPathFollowingRequestResult::Type MoveToTarget();

    virtual void CheckEnnemyNear(float range) PURE_VIRTUAL(ACustomAIControllerBase::CheckEnnemyNear, );

    virtual void CheckEnnemyNear(FVector position, float range) PURE_VIRTUAL(ACustomAIControllerBase::CheckEnnemyNear, );

    virtual void AttackTarget() const PURE_VIRTUAL(ACustomAIControllerBase::AttackTarget, );

    //aim at the fire direction and modify it needed.
    virtual void aim(FVector& inOutFireDirection) const USE_NOEXCEPT;
};
