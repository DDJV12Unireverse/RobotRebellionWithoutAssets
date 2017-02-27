// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "MoveToTargetBTTaskNode.h"
#include "EnnemiAIController.h"



UMoveToTargetBTTaskNode::UMoveToTargetBTTaskNode()
{
    NodeName = "MoveToTarget";
    bNotifyTick = true;
}

EBTNodeResult::Type UMoveToTargetBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
                                                         NodeMemory)
{
    AEnnemiAIController* ennemiAIController = Cast<AEnnemiAIController>(OwnerComp.GetOwner());

    EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

    // If the controller doesn't have a target, the task is a fail
    if(ennemiAIController->hasTarget())
    {
        EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
        EPathFollowingRequestResult::Type MoveToActorResult = ennemiAIController->MoveToTarget();
        if(MoveToActorResult == EPathFollowingRequestResult::AlreadyAtGoal)
        {
            NodeResult = EBTNodeResult::Succeeded;
        }
    }

    return NodeResult;
}

void UMoveToTargetBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                       float DeltaSeconds)
{
    AEnnemiAIController* ennemiAIController = Cast<AEnnemiAIController>(OwnerComp.GetOwner());
    if(ennemiAIController->hasTarget())
    {
        EPathFollowingRequestResult::Type MoveToActorResult = ennemiAIController->MoveToTarget();
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