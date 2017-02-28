// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "TrainingDummyCharacter.h"

#include "WeaponBase.h"
#include "WeaponInventory.h"


ATrainingDummyCharacter::ATrainingDummyCharacter() : ANonPlayableCharacter()
{
    // fill it

    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Robots");

    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}