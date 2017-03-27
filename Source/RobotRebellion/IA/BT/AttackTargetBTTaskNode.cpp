// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "AttackTargetBTTaskNode.h"
#include "../Controller/CustomAIControllerBase.h"
#include "Character/NonPlayableCharacter.h"


UAttackTargetBTTaskNode::UAttackTargetBTTaskNode()
{
    NodeName = "AttackTarget";
    bNotifyTick = true;
}

EBTNodeResult::Type UAttackTargetBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
                                                         NodeMemory)
{
    ACustomAIControllerBase* AIController = Cast<ACustomAIControllerBase>(OwnerComp.GetOwner());

    EBTNodeResult::Type NodeResult = EBTNodeResult::Succeeded;

    AIController->AttackTarget();

    return NodeResult;
}

void UAttackTargetBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                       float DeltaSeconds)
{
    ACustomAIControllerBase* AIController = Cast<ACustomAIControllerBase>(OwnerComp.GetOwner());
    AIController->AttackTarget();
}
FString UAttackTargetBTTaskNode::GetStaticDescription() const
{
    return TEXT("Attack the targeted target");
}
