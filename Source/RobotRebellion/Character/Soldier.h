// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayableCharacter.h"
#include "Soldier.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ASoldier : public APlayableCharacter
{
    GENERATED_BODY()


public:
    ASoldier();

    EClassType getClassType() const USE_NOEXCEPT override
    {
        return EClassType::SOLDIER;
    }
};
