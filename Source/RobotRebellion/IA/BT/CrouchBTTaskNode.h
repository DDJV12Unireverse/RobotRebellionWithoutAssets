// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CrouchBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UCrouchBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UCrouchBTTaskNode();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                            uint8* NodeMemory) override;
    virtual void TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                          float DeltaSeconds) override;
    virtual FString GetStaticDescription() const override;
};
