// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotsCharacter.h"

#include "WeaponBase.h"
#include "WeaponInventory.h"


ARobotsCharacter::ARobotsCharacter() : ANonPlayableCharacter()
{
    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Robots");

    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}
