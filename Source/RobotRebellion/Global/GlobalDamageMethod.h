// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Tool/IsSingleton.h"
#include "../Gameplay/Damage.h"
#include "../Character/RobotRebellionCharacter.h"
#include "../Gameplay/WeaponBase.h"

/**
 * 
 */
class UGlobalDamageMethod
{
public:
    static Damage::DamageValue normalHit(const ARobotRebellionCharacter* assailant, const ARobotRebellionCharacter* receiver);
    static Damage::DamageValue normalHitWithWeaponComputed(const ARobotRebellionCharacter* assailant, const ARobotRebellionCharacter* receiver);
	
};
