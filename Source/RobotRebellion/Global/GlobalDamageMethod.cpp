// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "GlobalDamageMethod.h"


Damage::DamageValue UGlobalDamageMethod::normalHit(const ARobotRebellionCharacter* assailant, const ARobotRebellionCharacter* receiver)
{
    float intermediary = assailant->getStrength() - receiver->getDefense();
    if (intermediary < 1.f)
    {
        intermediary = 1.f;
    }

    intermediary *= FMath::Sqrt(assailant->getAgility() / receiver->getAgility());

    return static_cast<Damage::DamageValue>(intermediary);
}


Damage::DamageValue UGlobalDamageMethod::normalHitWithWeaponComputed(const ARobotRebellionCharacter* assailant, const ARobotRebellionCharacter* receiver)
{
    Damage::DamageValue intermediary = UGlobalDamageMethod::normalHit(assailant, receiver);
    
    intermediary *= assailant->getCurrentEquippedWeapon()->m_weaponDamageCoefficient;
    intermediary += assailant->getCurrentEquippedWeapon()->m_weaponBaseDamage;

    return static_cast<Damage::DamageValue>(intermediary);
}

Damage::DamageValue UGlobalDamageMethod::droneDamageComputed(const ARobotRebellionCharacter* assailant, const ARobotRebellionCharacter* receiver)
{
    float intermediary = (assailant->getStrength() - receiver->getDefense()) * 10.f;
    if(intermediary < 1.f)
    {
        intermediary = 1.f;
    }

    return static_cast<Damage::DamageValue>(intermediary);
}
