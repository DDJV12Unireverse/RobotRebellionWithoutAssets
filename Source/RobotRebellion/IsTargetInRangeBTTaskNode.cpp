// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "IsTargetInRangeBTTaskNode.h"

#include "EnnemiAIController.h"


UIsTargetInRangeBTTaskNode::UIsTargetInRangeBTTaskNode()
{
}

EBTNodeResult::Type UIsTargetInRangeBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnnemiAIController* ennemiAIController = Cast<AEnnemiAIController>(OwnerComp.GetOwner());

    EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

    // If the controller doesn't have a target, the task is a fail
    if(ennemiAIController->hasTarget())
    {
        auto test = ennemiAIController->getTarget();
        PRINT_MESSAGE_ON_SCREEN(FColor::Green, TEXT("Not in range"));
    }

    return NodeResult;
}

void UIsTargetInRangeBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AEnnemiAIController* ennemiAIController = Cast<AEnnemiAIController>(OwnerComp.GetOwner());
    if(ennemiAIController->hasTarget())
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Green, TEXT("Not in range"));
    }
}

FString UIsTargetInRangeBTTaskNode::GetStaticDescription() const
{
    return TEXT("Attack the targeted target");
}