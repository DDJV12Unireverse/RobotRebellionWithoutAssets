// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Attributes.h"

void UAttributes::setMaxMana(AttributeULongType newValue) noexcept
{
    m_maxMana = newValue;

    if (newValue < m_maxMana)
    {
        m_maxMana = m_maxMana;
    }
}

void UAttributes::setMaxHealth(AttributeULongType newValue) noexcept
{
    m_maxHealth = newValue;

    if (newValue < m_health)
    {
        m_health = m_maxHealth;
    }
}

void UAttributes::inflictDamage(AttributeULongType damage) noexcept
{
    if (damage < m_health)
    {
        m_health -= damage;
    }
    else
    {
        m_health = 0;
    }
}