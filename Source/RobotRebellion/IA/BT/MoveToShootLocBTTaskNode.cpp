// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "MoveToShootLocBTTaskNode.h"
#include "../Controller/CustomAIControllerBase.h"
#include "../Controller/RobotShooterController.h"



UMoveToShootLocBTTaskNode::UMoveToShootLocBTTaskNode()
{
    NodeName = "MoveToShootLoc";
    bNotifyTick = true;
}

EBTNodeResult::Type UMoveToShootLocBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
                                                         NodeMemory)
{

    ACustomAIControllerBase* AIController = Cast<ACustomAIControllerBase>(OwnerComp.GetOwner());

    EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

    // If the controller doesn't have a target, the task is a fail
    if(AIController->hasALivingTarget())
    {
        NodeResult = EBTNodeResult::Succeeded;
        ARobotShooterController* shooterController = Cast<ARobotShooterController>(AIController);
        if(shooterController)
        {
            // update shoot position
            shooterController->updateShootLocation();
            shooterController->uncrouch();
            // TODO - Switch to moveToShootLocation!
            EPathFollowingRequestResult::Type MoveToActorResult = shooterController->moveToShootLocation();
        }
        else
        {
            EPathFollowingRequestResult::Type MoveToActorResult = AIController->MoveToTarget();
        }
    }

    return NodeResult;
}

void UMoveToShootLocBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
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
FString UMoveToShootLocBTTaskNode::GetStaticDescription() const
{
    return TEXT("Follow the targeted target");
}

