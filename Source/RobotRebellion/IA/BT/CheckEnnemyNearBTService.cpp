// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CheckEnnemyNearBTService.h"

#include "../Controller/CustomAIControllerBase.h"
#include "../Character/RobotsCharacter.h"


UCheckEnnemyNearBTService::UCheckEnnemyNearBTService() : m_radiusRange{700}
{
    NodeName = "CheckEnnemyNear";
    // Interval update
    Interval = 0.5f;
    // Random update deviation for update
    RandomDeviation = 0.1f;
}

void UCheckEnnemyNearBTService::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    ACustomAIControllerBase* AIController = Cast<ACustomAIControllerBase>(OwnerComp.GetOwner());
    if(!AIController->hasALivingTarget())
    {
        AIController->CheckEnnemyNear(m_radiusRange);
    }
    else if(AIController->getTarget() && !AIController->getTarget()->isVisible()) //loose Track
    {
        AIController->setTarget(nullptr);
        AIController->StopMovement();
    }
}
