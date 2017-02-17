// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RobotRebellionCharacter.h"
#include "Wizard.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API AWizard : public ARobotRebellionCharacter
{
	GENERATED_BODY()
	
        AWizard() :ARobotRebellionCharacter()
    {}
    classtype getClass() override
    {
        return WIZARD;
    }
	
	
};
