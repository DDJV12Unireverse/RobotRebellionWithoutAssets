// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotsCharacter.h"

#include "../../Gameplay/Weapon/WeaponInventory.h"


ARobotsCharacter::ARobotsCharacter() : ANonPlayableCharacter()
{
    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Robots");

    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}
