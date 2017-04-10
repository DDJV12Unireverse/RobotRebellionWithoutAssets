// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Spell/Effects/Effect.h"
#include "ShieldEffect.generated.h"

/**
 * Apply a shield on the target : can customize duration and amount
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API UShieldEffect : public UEffect
{
	GENERATED_BODY()

public:
    /** Specifie the shield amount (1 shield point <-> 1 health point)*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShieldEffect)
        float m_shieldAmount;

public:
    // Called when the game starts
    virtual void BeginPlay() override;
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // The behavior of the effect when it's a targeted effect
    virtual void exec(class ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target) override;
    // The behavior of the effect when it's point effect
    virtual void exec(const FVector impactPoint, ARobotRebellionCharacter* caster = nullptr) override;
};
