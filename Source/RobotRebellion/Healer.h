// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RobotRebellionCharacter.h"
#include "Healer.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API AHealer : public ARobotRebellionCharacter
{
	GENERATED_BODY()
	
        AHealer() :ARobotRebellionCharacter()
    {}
    classtype getClass() override
    {
        return HEALER;
    }
	
	
};
