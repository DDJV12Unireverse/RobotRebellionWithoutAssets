// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LifeBarWidget.h"



void ULifeBarWidget::getHealthRatio(float& ratio, float& ratioShield, float& health, float& shield, float& maxHealth)
{
    if(m_owner)
    {
        health = m_owner->getHealth();
        shield = m_owner->getShield();
        maxHealth = m_owner->getMaxHealth();
        ratio = health / maxHealth;
        ratioShield = (health + shield) / maxHealth;
    }
    else
    {
        health = 0.f;
        maxHealth = 0.f;
        ratio = 0.f;
    }
}

