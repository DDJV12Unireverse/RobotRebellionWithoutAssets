// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "IsTargetInRangeBTTaskNode.h"

#include "../Controller/CustomAIControllerBase.h"
#include "../../Character/RobotRebellionCharacter.h"
#include "../../Gameplay/Weapon/WeaponInventory.h"
#include "../../Gameplay/Weapon/WeaponBase.h"


UIsTargetInRangeBTTaskNode::UIsTargetInRangeBTTaskNode()
{
}

EBTNodeResult::Type UIsTargetInRangeBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ACustomAIControllerBase* AIController = Cast<ACustomAIControllerBase>(OwnerComp.GetOwner());

    EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

    // If the controller doesn't have a target, the task is a fail
    if(AIController->hasALivingTarget())
    {
        FVector currentTargetLocation = AIController->getTarget()->GetActorLocation();
        FVector ennemiLocation = AIController->GetPawn()->GetActorLocation();
        ARobotRebellionCharacter* ennemiCharacter = Cast<ARobotRebellionCharacter>(AIController->GetCharacter());

        FVector distanceBetween = currentTargetLocation - ennemiLocation;
        if(ennemiCharacter->m_weaponInventory)
        {
            if(distanceBetween.Size() < m_detectingRange // Ensure the target is at the correct distance
               && ennemiCharacter->m_weaponInventory->getCurrentWeapon()->canAttack()) // Ensure the AI weapon are reloaded
            {
                NodeResult = EBTNodeResult::Succeeded;
            }
        }

    }

    return NodeResult;
}

void UIsTargetInRangeBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ACustomAIControllerBase* AIController = Cast<ACustomAIControllerBase>(OwnerComp.GetOwner());
    if(AIController->hasALivingTarget())
    {
        FVector currentTargetLocation = AIController->getTarget()->GetActorLocation();
        FVector ennemiLocation = AIController->GetPawn()->GetActorLocation();

        FVector distanceBetween = currentTargetLocation - ennemiLocation;
    }
}

FString UIsTargetInRangeBTTaskNode::GetStaticDescription() const
{
    return TEXT("Attack the targeted target");
}