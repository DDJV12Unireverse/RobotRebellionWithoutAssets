// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotRebellionCharacter.h"
#include "CustomRobotRebellionUserWidget.h"


void UCustomRobotRebellionUserWidget::getHealthRatio(float& ratio, float& health, float& maxHealth) const
{
    ARobotRebellionCharacter* character = Cast<ARobotRebellionCharacter>(GetOwningPlayerPawn());

    if (character)
    {
        health = character->getHealth();
        maxHealth = character->getMaxHealth();
        ratio = health / maxHealth;
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

FString UCustomRobotRebellionUserWidget::healthParseToScreen(float health, float maxHealth) const
{
    return FString::FromInt(health) + TEXT("/") + FString::FromInt(maxHealth);
}

FString UCustomRobotRebellionUserWidget::manaParseToScreen(float mana, float maxMana) const
{
    return FString::FromInt(mana) + TEXT("/") + FString::FromInt(maxMana);
}
