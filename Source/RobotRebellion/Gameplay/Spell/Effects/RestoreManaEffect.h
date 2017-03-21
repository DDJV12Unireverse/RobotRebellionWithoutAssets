// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Spell/Effects/Effect.h"
#include "RestoreManaEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API URestoreManaEffect : public UEffect
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ManaEffect)
        float m_manaGiven;

public:
    // Called when the game starts
    virtual void BeginPlay() override;
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // The behavior of the effect when it's a targeted effect
    virtual void exec(class ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target) override;
    // The behavior of the effect when it's point effect
    virtual void exec(const FVector impactPoint) override;
	
};