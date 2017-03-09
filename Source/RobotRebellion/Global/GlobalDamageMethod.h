// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Tool/IsSingleton.h"
#include "../Gameplay/Damage/Damage.h"
#include "../Gameplay/Weapon/WeaponBase.h"
#include "../Character/RobotRebellionCharacter.h"

/**
 * 
 */
class UGlobalDamageMethod
{
public:
    static Damage::DamageValue normalHit(const ARobotRebellionCharacter* assailant, const ARobotRebellionCharacter* receiver);
    static Damage::DamageValue normalHitWithWeaponComputed(const ARobotRebellionCharacter* assailant, const ARobotRebellionCharacter* receiver);
	
};
