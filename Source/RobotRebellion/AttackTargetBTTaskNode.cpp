// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "AttackTargetBTTaskNode.h"
#include "EnnemiAIController.h"
#include "NonPlayableCharacter.h"


UAttackTargetBTTaskNode::UAttackTargetBTTaskNode()
{
    NodeName = "AttackTarget";
    bNotifyTick = true;
}

EBTNodeResult::Type UAttackTargetBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
                                                         NodeMemory)
{
    AEnnemiAIController* ennemiAIController = Cast<AEnnemiAIController>(OwnerComp.GetOwner());

    EBTNodeResult::Type NodeResult = EBTNodeResult::Succeeded;

    ennemiAIController->AttackTarget();

    return NodeResult;
}

void UAttackTargetBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                       float DeltaSeconds)
{
    AEnnemiAIController* ennemiAIController = Cast<AEnnemiAIController>(OwnerComp.GetOwner());
    ennemiAIController->AttackTarget();
}
FString UAttackTargetBTTaskNode::GetStaticDescription() const
{
    return TEXT("Attack the targeted target");
}
