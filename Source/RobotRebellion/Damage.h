// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Algorithm.h"
#include <Runtime/Core/Public/Math/UnrealMathUtility.h>

/**
 * 
 */
class DamageHelperConstants
{
public:
    static constexpr const float RANDOM_MIN_COEFFICIENT = 0.8f;
    static constexpr const float RANDOM_MAX_COEFFICIENT = 1.2f;
};

class ARobotRebellionCharacter;

class ROBOTREBELLION_API Damage
{
public:
    using CoefficientType = float;
    using DamageValue = unsigned int;


private:
    const ARobotRebellionCharacter* m_assailant;
    const ARobotRebellionCharacter* m_receiver;


public:
	Damage(const ARobotRebellionCharacter*const m_assailant, const ARobotRebellionCharacter*const m_receiver);
	~Damage();


public:
    template<class Func>
    DamageValue operator()(const Func& algorithm, CoefficientType damageCoefficient) const
    {
        Algorithm<Func> alg{ algorithm };

        return 
            damageCoefficient * 
            FMath::FRandRange(
                DamageHelperConstants::RANDOM_MIN_COEFFICIENT, 
                DamageHelperConstants::RANDOM_MAX_COEFFICIENT
            ) * alg.operator()<DamageValue>(m_assailant, m_receiver);
    }
};

