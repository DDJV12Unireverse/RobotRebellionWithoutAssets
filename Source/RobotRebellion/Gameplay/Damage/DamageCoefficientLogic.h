// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RobotRebellion.h"

/**
 * 
 */
class ROBOTREBELLION_API DamageCoefficientLogic
{
public:
    static constexpr const float CRITICAL_EFFECT_MULTIPLICATOR = 3.0f;
    static constexpr const float SUPER_EFFICIENT_EFFECT_MULTIPLICATOR = 1.5f;
    static constexpr const float MULTIPLE_HIT_EFFECT_MULTIPLICATOR = 0.3f;
    static constexpr const float ENGAGEMENT_EFFECT_MULTIPLICATOR = 1.1f;
    static constexpr const float BACKSTAB_EFFECT_MULTIPLICATOR = 1.2f;

    static constexpr const float MIN_COEFFICIENT_VALUE = 0.1f;



private:
    float m_damageCoefficient = 1.0f;



public:
    DamageCoefficientLogic() = default;

    constexpr DamageCoefficientLogic(float startingValue) : 
        m_damageCoefficient{ startingValue }
    {}

    ~DamageCoefficientLogic() = default;


    void multiplyCoefficient(float value) USE_NOEXCEPT
    {
        m_damageCoefficient *= value;
    }

    void divideCoefficient(float value) USE_NOEXCEPT
    {
        m_damageCoefficient /= value;
        if (m_damageCoefficient < MIN_COEFFICIENT_VALUE)
        {
            m_damageCoefficient = MIN_COEFFICIENT_VALUE;
        }
    }

    //Modify the damage coefficient according to the fact that the attack was a critical hit.
    void criticalHit() USE_NOEXCEPT
    {
        multiplyCoefficient(CRITICAL_EFFECT_MULTIPLICATOR);
    }

    //Modify the damage coefficient according to the fact that the attack was a backstab attack.
    void backstab() USE_NOEXCEPT
    {
        multiplyCoefficient(BACKSTAB_EFFECT_MULTIPLICATOR);
    }

    //Modify the damage coefficient according to the fact that the attack grazed the target.
    void graze() USE_NOEXCEPT
    {
        divideCoefficient(CRITICAL_EFFECT_MULTIPLICATOR);
    }

    //Modify the damage coefficient according to the fact that the attack was greatly efficient against the target.
    void superEfficient() USE_NOEXCEPT
    {
        multiplyCoefficient(SUPER_EFFICIENT_EFFECT_MULTIPLICATOR);
    }

    //Modify the damage coefficient according to the fact that the attack was less efficient against the target.
    void lessEfficient() USE_NOEXCEPT
    {
        divideCoefficient(SUPER_EFFICIENT_EFFECT_MULTIPLICATOR);
    }

    //Modify the damage coefficient according to the fact that the attack was a mutliple hit attack (reduce the unitary attack coefficient).
    void multipleHit() USE_NOEXCEPT
    {
        multiplyCoefficient(MULTIPLE_HIT_EFFECT_MULTIPLICATOR);
    }

    //Modify the damage coefficient according to the fact that the attack was an engagement hit.
    void engagementHit() USE_NOEXCEPT
    {
        multiplyCoefficient(ENGAGEMENT_EFFECT_MULTIPLICATOR);
    }

    //return the current coefficient value.
    float getCoefficientValue() const USE_NOEXCEPT
    {
        return m_damageCoefficient;
    }

    bool establishCritical(const FName& boneName) const USE_NOEXCEPT;
};
