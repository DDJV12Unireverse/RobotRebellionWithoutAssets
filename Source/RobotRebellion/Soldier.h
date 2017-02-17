// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RobotRebellionCharacter.h"
#include "Soldier.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ASoldier : public ARobotRebellionCharacter
{
    GENERATED_BODY()

        ASoldier() :ARobotRebellionCharacter()
    {}
    classtype getClass() override
    {
        return SOLDIER;
    }
	
	
};
