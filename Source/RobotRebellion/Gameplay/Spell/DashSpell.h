// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Spell/Spell.h"
#include "DashSpell.generated.h"

/**
 * Similar to the Ray cast spell
 * This kind of spell doesnt need to hit object to apply effect
 */
UCLASS()
class ROBOTREBELLION_API UDashSpell : public USpell
{
	GENERATED_BODY()
	
public:
    UDashSpell();

    virtual void BeginPlay() override;

    virtual void cast() override;

    // Apply Effects on a target that have to be a RobotRebellion Character
    void applyEffect(class ARobotRebellionCharacter* affectedTarget);
    // Aplly Effects on a specific location
    void applyEffect(FVector impactPoint);

private:
    // Return corrected aim direction
    FVector getRealAimingVector(const ARobotRebellionCharacter* caster);
	
};
