// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "GunTurretCharacter.h"


AGunTurretCharacter::AGunTurretCharacter()
{
    // Use same collision as player to allow hostiles to attack
    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Player"); 

    // TODO - initialize with the right weapon :)
    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}

