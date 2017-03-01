// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Spell.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API USpell : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        FString m_name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        float m_range;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        float m_cooldown;

private:
    float m_nextAllowedCastTimer;


public:
    USpell();


    void cast();

};
