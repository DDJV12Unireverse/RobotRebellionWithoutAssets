// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "IsTargetInRangeBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UIsTargetInRangeBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIBT | Settings")
        float m_detectingRange;

    UIsTargetInRangeBTTaskNode();


    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
                                            uint8* NodeMemory) override;
    virtual void TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                          float DeltaSeconds) override;
    virtual FString GetStaticDescription() const override;
};
