// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RobotRebellionWidget.h"
#include "LifeBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ULifeBarWidget : public URobotRebellionWidget
{
	GENERATED_BODY()
	
public:
        UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        void getHealthRatio(float& ratio, float& ratioShield, float& health, float& shield, float& maxHealth) const;
	
};
