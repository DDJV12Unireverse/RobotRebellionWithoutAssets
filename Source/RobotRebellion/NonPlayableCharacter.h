// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RobotRebellionCharacter.h"
#include "NonPlayableCharacter.generated.h"

/**
 * Mother class for every npc in RobotRebellion Game
 */
UCLASS()
class ROBOTREBELLION_API ANonPlayableCharacter : public ARobotRebellionCharacter
{
	GENERATED_BODY()

public:
    ANonPlayableCharacter();
	
    virtual void cppOnDeath() override;
};
