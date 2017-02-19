// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "IWeaponBase.h"
#include "Engine/World.h"



UIWeaponBase::UIWeaponBase() :
    m_weaponDamageCoefficient{ 1.f },
    m_weaponBaseDamage{ 0.f },
    m_weaponBaseCadence{ 1.f },
    m_canAttack{ false }
{
    bReplicates = true;
}

void UIWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UIWeaponBase, m_canAttack, COND_OwnerOnly);
}

void UIWeaponBase::reload()
{
    if (!m_canAttack)
    {
        m_canAttack = true;
        GetWorld()->GetTimerManager().ClearTimer(m_nextAllowedAttackTimer);
    }
}

void UIWeaponBase::waitForReloading()
{
    if (m_canAttack)
    {
        m_canAttack = false;
        FTimerManager::ValidateHandle(m_nextAllowedAttackTimer);
        GetWorld()->GetTimerManager().SetTimer(m_nextAllowedAttackTimer, this, &UIWeaponBase::reload, m_weaponBaseCadence, false);
    }
}

FString UIWeaponBase::toDebugFString() const USE_NOEXCEPT
{
    return TEXT("Damage coefficient multiplier : ") + 
        FString::FromInt(m_weaponDamageCoefficient) +
        TEXT("weapon base damage : ") +
        FString::FromInt(m_weaponBaseDamage);
}
