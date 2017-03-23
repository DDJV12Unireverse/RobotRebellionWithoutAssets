// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Spell/Spell.h"
#include "SelfSpell.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API USelfSpell : public USpell
{
	GENERATED_BODY()
	
public:
    USelfSpell();

    virtual void BeginPlay() override;

    virtual void cast() override;

    // Call by the projectile once it hit smth
    void onHit(class UPrimitiveComponent*, class AActor*, class UPrimitiveComponent*, FVector, const FHitResult&)
    {}

    // Apply Effects on a target that have to be a RobotRebellion Character
    void applyEffect(class ARobotRebellionCharacter* affectedTarget);
    // Aplly Effects on a specific location
    void applyEffect(FVector impactPoint)
    {}	
	
};
