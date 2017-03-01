// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CustomAIControllerBase.h"
#include "EnnemiAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API AEnnemiAIController : public ACustomAIControllerBase
{
    GENERATED_BODY()

public:
    void CheckEnnemyNear(float range);

    void AttackTarget() const;
	
};
