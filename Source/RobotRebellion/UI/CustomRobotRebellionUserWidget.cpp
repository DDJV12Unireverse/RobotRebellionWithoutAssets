// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CustomRobotRebellionUserWidget.h"
#include "Character/RobotRebellionCharacter.h"


void UCustomRobotRebellionUserWidget::getHealthRatio(float& ratio, float& ratioShield, float& health, float& shield, float& maxHealth) const
{
    ARobotRebellionCharacter* character = Cast<ARobotRebellionCharacter>(GetOwningPlayerPawn());

    if (character)
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

void UCustomRobotRebellionUserWidget::getManaRatio(float& ratio, float& mana, float& maxMana) const
{
    ARobotRebellionCharacter* character = Cast<ARobotRebellionCharacter>(GetOwningPlayerPawn());

    if (character)
    {
        mana = character->getMana();
        maxMana = character->getMaxMana();
        ratio = mana / maxMana;
    }
    else
    {
        mana = 0.f;
        maxMana = 0.f;
        ratio = 0.f;
    }
}

FString UCustomRobotRebellionUserWidget::healthParseToScreen(float health, float shield, float maxHealth) const
{
    if(shield > 0)
    {
        return FString::FromInt(health) + TEXT("(") + FString::FromInt(shield) + TEXT(")") + TEXT("/") + FString::FromInt(maxHealth);
    }
    else
    {
        return FString::FromInt(health) + TEXT("/") + FString::FromInt(maxHealth);
    }
}

FString UCustomRobotRebellionUserWidget::manaParseToScreen(float mana, float maxMana) const
{
    return FString::FromInt(mana) + TEXT("/") + FString::FromInt(maxMana);
}
