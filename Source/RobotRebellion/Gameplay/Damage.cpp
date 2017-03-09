// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Damage.h"
#include "../Character/RobotRebellionCharacter.h"



Damage::Damage(const ARobotRebellionCharacter*const assailant, const ARobotRebellionCharacter*const receiver) :
    m_assailant{ assailant },
    m_receiver{ receiver }
{
}

Damage::~Damage()
{
}
