// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "TrainingDummyCharacter.h"

#include "../Gameplay/WeaponBase.h"
#include "../Gameplay/WeaponInventory.h"


ATrainingDummyCharacter::ATrainingDummyCharacter() : ANonPlayableCharacter()
{
    // fill it

    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Robots");

    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}