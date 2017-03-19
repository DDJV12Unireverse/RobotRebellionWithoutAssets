// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Spell/Spell.h"
#include "RayCastSpell.generated.h"

/**
 * Implemente logic for spell that doesnt have projectile and stuff
 *   It will just use rayCast to know how the spell is cast.
 */
UCLASS()
class ROBOTREBELLION_API URayCastSpell : public USpell
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast Settings")
        bool m_isTargetThrow;

public:
    URayCastSpell();

    virtual void BeginPlay() override;

    virtual void cast() override;

    // Apply Effects on a target that have to be a RobotRebellion Character
    void applyEffect(class ARobotRebellionCharacter* affectedTarget);
    // Aplly Effects on a specific location
    void applyEffect(FVector impactPoint);

};
