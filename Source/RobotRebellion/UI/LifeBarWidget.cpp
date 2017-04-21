// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LifeBarWidget.h"



void ULifeBarWidget::getHealthRatio(float& ratio, float& ratioShield, float& health, float& shield, float& maxHealth) const
{
    ARobotRebellionCharacter* character = Cast<ARobotRebellionCharacter>(this->GetOwningPlayerPawn());

    if(character)
    {
        health = character->getHealth();
        shield = character->getShield();
        maxHealth = character->getMaxHealth();
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

