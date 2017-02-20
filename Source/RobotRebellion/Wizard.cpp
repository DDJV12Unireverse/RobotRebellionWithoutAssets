// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Wizard.h"


AWizard::AWizard() :ARobotRebellionCharacter()
{
    m_attribute->setStrength(8.5f);
    m_attribute->setAgility(6.5f);
    m_attribute->setDefense(3.0f);
    m_attribute->setHealth(4.0f);
    m_attribute->setMana(8.0f);
    m_attribute->setMaxHealth(4.0f);
    m_attribute->setMaxMana(8.0f);
}

