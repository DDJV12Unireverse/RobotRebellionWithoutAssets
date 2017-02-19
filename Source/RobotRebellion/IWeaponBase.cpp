// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "IWeaponBase.h"
#include "Engine/World.h"



UIWeaponBase::UIWeaponBase() :
    m_weaponDamageCoefficient{ 1.f },
    m_weaponBaseDamage{ 0.f },
    m_weaponBaseCadence{ 1.f },
    m_nextAllowedAttackTimer{ 0.0 }
{
    bReplicates = true;
}

void UIWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UIWeaponBase::canAttack() const USE_NOEXCEPT
{
    return FPlatformTime::Seconds() > m_nextAllowedAttackTimer;
}

void UIWeaponBase::reload()
{
    m_nextAllowedAttackTimer = FPlatformTime::Seconds() + m_weaponBaseCadence;
}

FString UIWeaponBase::toDebugFString() const USE_NOEXCEPT
{
    return TEXT("Damage coefficient multiplier : ") + 
        FString::FromInt(m_weaponDamageCoefficient) +
        TEXT("weapon base damage : ") +
        FString::FromInt(m_weaponBaseDamage);
}
