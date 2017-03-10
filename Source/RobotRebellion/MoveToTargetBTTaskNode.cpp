// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "MoveToTargetBTTaskNode.h"
#include "CustomAIControllerBase.h"



UMoveToTargetBTTaskNode::UMoveToTargetBTTaskNode()
{
    NodeName = "MoveToTarget";
    bNotifyTick = true;
}

EBTNodeResult::Type UMoveToTargetBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
                                                         NodeMemory)
{
    ACustomAIControllerBase* AIController = Cast<ACustomAIControllerBase>(OwnerComp.GetOwner());

    EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

    // If the controller doesn't have a target, the task is a fail
    if(AIController->hasALivingTarget())
    {
        NodeResult = EBTNodeResult::Succeeded;
        EPathFollowingRequestResult::Type MoveToActorResult = AIController->MoveToTarget();
    }

    return NodeResult;
}

void UMoveToTargetBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                       float DeltaSeconds)
{
    ACustomAIControllerBase* AIController = Cast<ACustomAIControllerBase>(OwnerComp.GetOwner());
    if(AIController->hasTarget())
    {
        EPathFollowingRequestResult::Type MoveToActorResult = AIController->MoveToTarget();
        if(MoveToActorResult == EPathFollowingRequestResult::AlreadyAtGoal)
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    }
}
FString UMoveToTargetBTTaskNode::GetStaticDescription() const
{
    return TEXT("Follow the targeted target");
}