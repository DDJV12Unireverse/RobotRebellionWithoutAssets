// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Attributes.h"
#include "Soldier.h"


ASoldier::ASoldier() : ARobotRebellionCharacter()
{
    m_attribute->setStrength(4.5f);
    m_attribute->setAgility(8.0f);
    m_attribute->setDefense(4.0f);
    m_attribute->setHealth(5.5f);
    m_attribute->setMana(8.0f);
    m_attribute->setMaxHealth(5.5f);
    m_attribute->setMaxMana(8.0f);
}


