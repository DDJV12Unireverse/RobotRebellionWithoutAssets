// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "WeaponBase.h"
#include "Engine/World.h"



UWeaponBase::UWeaponBase() :
    m_weaponDamageCoefficient{ 1.f },
    m_weaponBaseDamage{ 0.f },
    m_weaponBaseCadence{ 1.f },
    m_nextAllowedAttackTimer{ 0.0 }, 
    m_WeaponRadiusRange{ 700.0 }
{
    bReplicates = true;
}

void UWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UWeaponBase::canAttack() const USE_NOEXCEPT
{
    return FPlatformTime::Seconds() > m_nextAllowedAttackTimer;
}

void UWeaponBase::reload()
{
    m_nextAllowedAttackTimer = FPlatformTime::Seconds() + m_weaponBaseCadence;
}

FString UWeaponBase::toDebugFString() const USE_NOEXCEPT
{
    return TEXT("Damage coefficient multiplier : ") + 
        FString::FromInt(m_weaponDamageCoefficient) +
        TEXT("weapon base damage : ") +
        FString::FromInt(m_weaponBaseDamage);
}

FString UWeaponBase::rangeToFString() const USE_NOEXCEPT
{
    return "Invalid weapon";
}