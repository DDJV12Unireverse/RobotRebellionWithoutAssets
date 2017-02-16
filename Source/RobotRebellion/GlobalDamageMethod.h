// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IsSingleton.h"
#include "Damage.h"
#include "RobotRebellionCharacter.h"

/**
 * 
 */
class UGlobalDamageMethod
{
public:
    static Damage::DamageValue normalHit(const ARobotRebellionCharacter* assailant, const ARobotRebellionCharacter* receiver);
	
};
