// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "IsTargetInRangeBTTaskNode.h"

#include "../Controller/EnnemiAIController.h"
#include "../../Character/RobotRebellionCharacter.h"
#include "../../Gameplay/Weapon/WeaponInventory.h"
#include "../../Gameplay/Weapon/WeaponBase.h"


UIsTargetInRangeBTTaskNode::UIsTargetInRangeBTTaskNode()
{
}

EBTNodeResult::Type UIsTargetInRangeBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnnemiAIController* ennemiAIController = Cast<AEnnemiAIController>(OwnerComp.GetOwner());

    EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

    // If the controller doesn't have a target, the task is a fail
    if(ennemiAIController->hasALivingTarget())
    {
        FVector currentTargetLocation = ennemiAIController->getTarget()->GetActorLocation();
        FVector ennemiLocation = ennemiAIController->GetPawn()->GetActorLocation();
        ARobotRebellionCharacter* ennemiCharacter = Cast<ARobotRebellionCharacter>(ennemiAIController->GetCharacter());

        FVector distanceBetween = currentTargetLocation - ennemiLocation;

        if(distanceBetween.Size() < m_detectingRange // Ensure the target is at the correct distance
            && ennemiCharacter->m_weaponInventory->getCurrentWeapon()->canAttack()) // Ensure the AI weapon are reloaded
        {
            NodeResult = EBTNodeResult::Succeeded;
        }
    }

    return NodeResult;
}

void UIsTargetInRangeBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AEnnemiAIController* ennemiAIController = Cast<AEnnemiAIController>(OwnerComp.GetOwner());
    if(ennemiAIController->hasALivingTarget())
    {
        FVector currentTargetLocation = ennemiAIController->getTarget()->GetActorLocation();
        FVector ennemiLocation = ennemiAIController->GetPawn()->GetActorLocation();

        FVector distanceBetween = currentTargetLocation - ennemiLocation;
    }
}

FString UIsTargetInRangeBTTaskNode::GetStaticDescription() const
{
    return TEXT("Attack the targeted target");
}