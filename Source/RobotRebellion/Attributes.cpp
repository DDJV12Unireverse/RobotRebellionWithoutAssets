// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Attributes.h"

unsigned int Attributes::setMaxMana(unsigned int newValue) noexcept
{
    m_maxMana = newValue;

    if (newValue < m_maxMana)
    {
        m_maxMana = m_maxMana;
    }
}

unsigned int Attributes::setMaxHealth(unsigned int newValue) noexcept
{
    m_maxHealth = newValue;

    if (newValue < m_health)
    {
        m_health = m_maxHealth;
    }
}
