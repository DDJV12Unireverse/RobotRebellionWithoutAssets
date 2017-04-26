// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RobotRebellionWidget.h"
#include "CustomRobotRebellionUserWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API UCustomRobotRebellionUserWidget : public URobotRebellionWidget
{
	GENERATED_BODY()
private:
    FString processFloatCooldown(float value) const;

public:
    //For blueprint : 4 output : ratio, health, shield and maxHealth
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        void getHealthRatio(float& ratio, float& ratioShield, float& health, float& shield, float& maxHealth) const;

    //For blueprint : 3 output : ratio, mana and maxMana
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        void getManaRatio(float& ratio, float& mana, float& maxMana) const;

    //Parse into FString = health / healthMax
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        FString healthParseToScreen(float health, float shield, float maxHealth) const;

    //Parse into FString = mana / manaMax
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        FString manaParseToScreen(float mana, float maxMana) const;

    // Parse cooldown into cooldown ofr every spell
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        TArray<FString> cooldownParseToScreen() const;

};
