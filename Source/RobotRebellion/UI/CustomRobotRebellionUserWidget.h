// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RobotRebellionWidget.h"
#include "Character/ClassType.h" 
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
    UFUNCTION(BlueprintNativeEvent, Category = "UpdateMethod")
        void updateClass(EClassType classType);

    //For blueprint : 4 output : ratio, health, shield and maxHealth
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        EClassType getPlayerClass() const;

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

    // Parse cooldown into cooldown for every spell
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        TArray<FString> cooldownParseToScreen() const;

    // Parse cooldown into 
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        FString bombParseToScreen() const;

    // Parse health pots amount 
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        FString healthPotsParseToScreen() const;
    
    // Parse mana pots amount 
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        FString manaPotsParseToScreen() const;

    // Check witch weapon is equipped return true if it's the main
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        bool isMainWeaponEquipped()const;
};  
