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


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
        bool m_showDebugSphereTrace;


public:
    bool m_isInCombat;
	FVector m_hitDirection;

public:
    ACustomAIControllerBase();
    virtual ~ACustomAIControllerBase() = default;
	
    virtual void BeginPlay() override;

    FORCEINLINE bool hasTarget() const USE_NOEXCEPT
    {
        return m_targetToFollow != NULL;
    }

    ARobotRebellionCharacter* getTarget() const USE_NOEXCEPT
    {
        return m_targetToFollow;
    }

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

    bool IsInCombat()
    {
        return m_isInCombat;
    }

    //Set in combat mode and give information from combat direction
    void setCombat(bool isCombat, ARobotRebellionCharacter* attacker);
};
