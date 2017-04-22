// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotShooterController.h"





void ARobotShooterController::CheckEnnemyNear(float range)
{
    DrawDebugSphere(GetPawn()->GetWorld(),
                    GetPawn()->GetActorLocation(),
                    range,
                    32,
                    FColor::Cyan,
                    false,
                    5.f, 0, 5.f);
    AEnnemiAIController::CheckEnnemyNear(range);
}

void ARobotShooterController::AttackTarget() const
{
    AEnnemiAIController::AttackTarget();
}
