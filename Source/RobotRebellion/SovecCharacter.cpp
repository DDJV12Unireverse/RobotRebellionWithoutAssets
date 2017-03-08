// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "SovecCharacter.h"

#include "WeaponInventory.h"


ASovecCharacter::ASovecCharacter()
{
    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Sovec");

    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}
