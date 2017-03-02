// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Spell.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API USpell : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        FString m_name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        float m_range;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        float m_cooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        TArray<TSubclassOf<class UEffect>> m_effectsClass;

private:
    float m_nextAllowedCastTimer;

    TArray<UEffect *> m_effects;

public:
    USpell();
    
    virtual void cast() ;

    virtual void BeginPlay() override;
};
