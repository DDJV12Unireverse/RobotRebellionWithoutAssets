// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "GlobalDamageMethod.h"


Damage::DamageValue UGlobalDamageMethod::normalHit(const ARobotRebellionCharacter* assailant, const ARobotRebellionCharacter* receiver)
{
    float intermediary = static_cast<float>(assailant->getStrength()) - static_cast<float>(receiver->getDefense());
    if (intermediary < 1.f)
    {
        intermediary = 1.f;
    }

    return static_cast<Damage::DamageValue>(static_cast<float>(assailant->getAgility()) / static_cast<float>(receiver->getAgility()) * intermediary);
}

