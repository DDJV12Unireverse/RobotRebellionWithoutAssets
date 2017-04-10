// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayableCharacter.h"
#include "Wizard.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API AWizard : public APlayableCharacter
{
	GENERATED_BODY()
	

public:
    AWizard();

    EClassType getClassType() const USE_NOEXCEPT override
    {
        return EClassType::WIZARD;
    }
};
