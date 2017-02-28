// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "CheckEnnemyNearBTService.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UCheckEnnemyNearBTService : public UBTService
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check Radius Settings")
        float m_radiusRange;

    UCheckEnnemyNearBTService();
	
    /** Will be called at each tick update */
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                          float DeltaSeconds) override;
	
};
