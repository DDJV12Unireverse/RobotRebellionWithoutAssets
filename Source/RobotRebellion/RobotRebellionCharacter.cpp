// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RobotRebellion.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "RobotRebellionCharacter.h"

#include "WeaponBase.h"
#include "WeaponInventory.h"


#include "UtilitaryMacros.h"



ARobotRebellionCharacter::ARobotRebellionCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    m_attribute = CreateDefaultSubobject<UAttributes>(TEXT("Attributes"));

    //m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}

void ARobotRebellionCharacter::BeginPlay()
{
    Super::BeginPlay();
}

///// SERVER
void ARobotRebellionCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ARobotRebellionCharacter, m_attribute);
}

UWeaponBase* ARobotRebellionCharacter::getCurrentEquippedWeapon() const USE_NOEXCEPT
{
    return m_weaponInventory->getCurrentWeapon();
}

void ARobotRebellionCharacter::cppOnDeath()
{}

void ARobotRebellionCharacter::onDeath()
{
    if (Role == ROLE_Authority)
    {
        clientOnDeath();
        //return;
    }
        
    this->cppOnDeath();
}

void ARobotRebellionCharacter::clientOnDeath_Implementation()
{
    this->cppOnDeath();
}

bool ARobotRebellionCharacter::clientOnDeath_Validate()
{
    return true;
}

void ARobotRebellionCharacter::cppOnRevive()
{}
