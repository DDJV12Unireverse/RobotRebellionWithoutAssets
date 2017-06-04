// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CrouchBTTaskNode.h"





#include "RobotRebellion.h"
#include "MoveToShootLocBTTaskNode.h"
#include "../Controller/CustomAIControllerBase.h"
#include "../Controller/RobotShooterController.h"



UCrouchBTTaskNode::UCrouchBTTaskNode()
{
    NodeName = "Crouch";
    bNotifyTick = true;
}

EBTNodeResult::Type UCrouchBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
                                                           NodeMemory)
{
    ARobotShooterController* AIController = Cast<ARobotShooterController>(OwnerComp.GetOwner());
    if(AIController->isCrouch())
    {
        return EBTNodeResult::Succeeded;
    }
    AIController->crouch();
    return EBTNodeResult::Succeeded;
}

void UCrouchBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                         float DeltaSeconds)
{
    ARobotShooterController* AIController = Cast<ARobotShooterController>(OwnerComp.GetOwner());

    if(AIController->isCrouch())
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
FString UCrouchBTTaskNode::GetStaticDescription() const
{
    return TEXT("Action bloc that make owner crouch - only work with RobotShooterController atm");
}

