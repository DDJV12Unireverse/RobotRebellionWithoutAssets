// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "MoveToTargetBTTaskNode.h"
#include "../Controller/EnnemiAIController.h"



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
        NodeResult = EBTNodeResult::Succeeded;
        EPathFollowingRequestResult::Type MoveToActorResult = ennemiAIController->MoveToTarget();
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