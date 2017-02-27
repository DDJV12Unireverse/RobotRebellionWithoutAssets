// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayableCharacter.h"
#include "Healer.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API AHealer : public APlayableCharacter
{
	GENERATED_BODY()
	

public:
    AHealer();

    EClassType getClassType() const USE_NOEXCEPT override
    {
        return EClassType::HEALER;
    }	
};
