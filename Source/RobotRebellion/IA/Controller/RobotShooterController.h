// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IA/Controller/EnnemiAIController.h"
#include "RobotShooterController.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ARobotShooterController : public AEnnemiAIController
{
	GENERATED_BODY()

public:

    void CheckEnnemyNear(float range) override;

    void AttackTarget() const override;
};
