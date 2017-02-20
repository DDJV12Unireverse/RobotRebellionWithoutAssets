// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RobotRebellionCharacter.h"
#include "Assassin.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API AAssassin : public ARobotRebellionCharacter
{
	GENERATED_BODY()
	

public:
    AAssassin();

    EClassType getClassType() const USE_NOEXCEPT override
    {
        return EClassType::ASSASSIN;
    }
};
