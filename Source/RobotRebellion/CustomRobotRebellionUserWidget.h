// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CustomRobotRebellionUserWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API UCustomRobotRebellionUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        void getHealthRatio(float& ratio, float& health, float& maxHealth) const;

    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        void getManaRatio(float& ratio, float& mana, float& maxMana) const;

    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        FString healthParseToScreen(float health, float maxHealth) const;

    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        FString manaParseToScreen(float mana, float maxMana) const;
};
