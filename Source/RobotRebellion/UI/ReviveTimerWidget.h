// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RobotRebellionWidget.h"
#include "ReviveTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UReviveTimerWidget : public URobotRebellionWidget
{
	GENERATED_BODY()
	
public:
    //For blueprint : 3 output : ratio, health and maxHealth
    UFUNCTION(BlueprintCallable, Category = "Update Methods")
        void getTimerRatio(float& ratio, float& currentTime, float& requiredTime) const;
};
