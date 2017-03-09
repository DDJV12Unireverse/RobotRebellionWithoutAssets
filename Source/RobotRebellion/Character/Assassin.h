// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayableCharacter.h"
#include "Assassin.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API AAssassin : public APlayableCharacter
{
	GENERATED_BODY()
	

public:
    AAssassin();

    //virtual void BeginPlay() override;

    EClassType getClassType() const USE_NOEXCEPT override
    {
        return EClassType::ASSASSIN;
    }
};
