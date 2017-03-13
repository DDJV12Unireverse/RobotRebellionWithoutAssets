// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "BeastCharacter.h"

#include "../../Gameplay/Weapon/WeaponInventory.h"

ABeastCharacter::ABeastCharacter()
{
    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Beasts");

    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}
