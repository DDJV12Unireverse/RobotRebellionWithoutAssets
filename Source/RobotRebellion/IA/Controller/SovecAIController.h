// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CustomAIControllerBase.h"
#include "SovecAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ASovecAIController : public ACustomAIControllerBase
{
	GENERATED_BODY()
	
public:

    void CheckEnnemyNear(float range) override;

    void AttackTarget() const override;
	
};
