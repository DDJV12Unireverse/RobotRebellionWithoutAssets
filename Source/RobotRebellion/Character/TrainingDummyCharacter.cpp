// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "TrainingDummyCharacter.h"

#include "../Gameplay/Weapon/WeaponBase.h"
#include "../Gameplay/Weapon/WeaponInventory.h"


ATrainingDummyCharacter::ATrainingDummyCharacter() : ANonPlayableCharacter()
{
    // fill it

    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Robots");

    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}