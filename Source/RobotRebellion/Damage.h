// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Algorithm.h"

/**
 * 
 */
class ROBOTREBELLION_API Damage
{
private:
    class ARobotRebellionCharacter;


public:
    using CoefficientType = float;
    using DamageValue = unsigned int;


private:
    ARobotRebellionCharacter* m_assailant;
    ARobotRebellionCharacter* m_receiver;


public:
	Damage(ARobotRebellionCharacter* m_assailant, ARobotRebellionCharacter* m_receiver);
	~Damage();


public:
    template<class Func>
    DamageValue operator()(Func& algorithm, CoefficientType damageCoefficient)
    {
        Algorithm<Func> alg{ algorithm };
        return (*static_cast<Algorithm<Func>*>(alg))(damageCoefficient, m_assailant, m_receiver);
    }
};

