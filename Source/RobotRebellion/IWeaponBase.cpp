// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "IWeaponBase.h"




FString UIWeaponBase::toFStringDebug() const USE_NOEXCEPT
{
    return TEXT("Damage coefficient multiplier : ") + 
        FString::FromInt(m_weaponDamageCoefficient) +
        TEXT("weapon base damage : ") +
        FString::FromInt(m_weaponBaseDamage);
}