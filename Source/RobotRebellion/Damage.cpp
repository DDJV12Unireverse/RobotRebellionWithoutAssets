// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Damage.h"
#include "RobotRebellionCharacter.h"



class DamageHelperConstants
{
public:
    static constexpr const float RANDOM_MIN_COEFFICIENT = 0.8f;
    static constexpr const float RANDOM_MAX_COEFFICIENT = 1.2f;
};

Damage::Damage(ARobotRebellionCharacter* assailant, ARobotRebellionCharacter* receiver) :
    m_assailant{ assailant },
    m_receiver{ receiver }
{
    //FMath::FRandRange();
}

Damage::~Damage()
{
}
