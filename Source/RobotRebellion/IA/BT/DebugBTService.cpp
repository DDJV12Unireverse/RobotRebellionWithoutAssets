#include "RobotRebellion.h"
#include "DebugBTService.h"

#include "../Controller/RobotShooterController.h"


UDebugBTService::UDebugBTService()
{
    NodeName = "Debug";
    // Interval update
    Interval = 2.0f;
    // Random update deviation for update
    RandomDeviation = 0.f;
}

void UDebugBTService::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    ARobotShooterController* AIController = Cast<ARobotShooterController>(OwnerComp.GetOwner());
    AIController->drawDebug();
}// Fill out your copyright notice in the Description page of Project Settings.






