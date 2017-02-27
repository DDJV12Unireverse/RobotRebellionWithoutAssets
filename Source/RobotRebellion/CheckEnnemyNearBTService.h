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
    UCheckEnnemyNearBTService();
    /** Sera appelé à chaque «update» du service */
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                          float DeltaSeconds) override;
	
};
