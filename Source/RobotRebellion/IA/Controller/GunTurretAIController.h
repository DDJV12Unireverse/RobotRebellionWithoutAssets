// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IA/Controller/CustomAIControllerBase.h"
#include "GunTurretAIController.generated.h"

/**
 * AI controller for the soldiers Gun Turret. attack only ennemy : Beast, Robot
 */
UCLASS()
class ROBOTREBELLION_API AGunTurretAIController : public ACustomAIControllerBase
{
	GENERATED_BODY()
	
public:

    void CheckEnnemyNear(float range) override;

    void AttackTarget() const override;
	
	
};
