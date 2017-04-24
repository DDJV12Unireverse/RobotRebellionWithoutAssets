// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "MoveToShootLocBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UMoveToShootLocBTTaskNode : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UMoveToShootLocBTTaskNode();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                            uint8* NodeMemory) override;
    virtual void TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                          float DeltaSeconds) override;
    virtual FString GetStaticDescription() const override;
};
