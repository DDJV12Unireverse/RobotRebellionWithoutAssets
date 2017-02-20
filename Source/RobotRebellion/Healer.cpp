// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Healer.h"


AHealer::AHealer():ARobotRebellionCharacter()
{
    m_attribute->setStrength(3.0f);
    m_attribute->setAgility(7.0f);
    m_attribute->setDefense(5.0f);
    m_attribute->setHealth(5.0f);
    m_attribute->setMana(10.0f);
    m_attribute->setMaxHealth(5.0f);
    m_attribute->setMaxMana(10.0f);
}
