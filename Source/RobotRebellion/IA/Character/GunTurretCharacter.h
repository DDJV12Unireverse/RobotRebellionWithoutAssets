// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/NonPlayableCharacter.h"
#include "GunTurretCharacter.generated.h"

/**
 * Soldier's Gun Turret character, used to initialize weaponInventory as we need
 */
UCLASS()
class ROBOTREBELLION_API AGunTurretCharacter : public ANonPlayableCharacter
{
	GENERATED_BODY()

public:
    AGunTurretCharacter();
};
