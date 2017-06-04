// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "DebugBTService.generated.h"

/**
 * Debug service for AIcontroller 
 * TODO - make it usable by every AIController by adding drawDebug virtual methode in Custom Ai Controller.
 * Now only used by RobotShooterController.
 */
UCLASS()
class ROBOTREBELLION_API UDebugBTService : public UBTService
{
	GENERATED_BODY()

public:
    UDebugBTService();

    /** Will be called at each tick update */
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                          float DeltaSeconds) override;
};
