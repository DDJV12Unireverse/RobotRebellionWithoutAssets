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
    //For blueprint : 3 output : ratio, health and maxHealth
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        void getHealthRatio(float& ratio, float& health, float& maxHealth) const;

    //For blueprint : 3 output : ratio, mana and maxMana
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        void getManaRatio(float& ratio, float& mana, float& maxMana) const;

    //Parse into FString = health / healthMax
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        FString healthParseToScreen(float health, float maxHealth) const;

    //Parse into FString = mana / manaMax
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        FString manaParseToScreen(float mana, float maxMana) const;
};
