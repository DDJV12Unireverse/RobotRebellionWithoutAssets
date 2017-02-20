// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Assassin.h"

AAssassin::AAssassin():ARobotRebellionCharacter()
{
    m_attribute->setStrength(6.5f);
    m_attribute->setAgility(10.0f);
    m_attribute->setDefense(3.5f);
    m_attribute->setHealth(4.5f);
    m_attribute->setMana(5.5f);
    m_attribute->setMaxHealth(4.5f);
    m_attribute->setMaxMana(5.5f);
}


