// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CustomRobotRebellionUserWidget.h"
#include "Character/RobotRebellionCharacter.h"
#include "Character/PlayableCharacter.h"
#include "Gameplay/Spell/SpellKit.h"
#include "Kismet/KismetMathLibrary.h"

void UCustomRobotRebellionUserWidget::updateClass_Implementation(EClassType classType)
{
    // Does Nothing
}

EClassType UCustomRobotRebellionUserWidget::getPlayerClass() const
{
    APlayableCharacter* character = Cast<APlayableCharacter>(GetOwningPlayerPawn());
    if(character)
    {
        return character->getClassType();
    }
    else
    {
        return EClassType::NONE;
    }
}

void UCustomRobotRebellionUserWidget::getHealthRatio(float& ratio, float& ratioShield, float& health, float& shield, float& maxHealth) const
{
    ARobotRebellionCharacter* character = Cast<ARobotRebellionCharacter>(GetOwningPlayerPawn());

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

void UCustomRobotRebellionUserWidget::getManaRatio(float& ratio, float& mana, float& maxMana) const
{
    ARobotRebellionCharacter* character = Cast<ARobotRebellionCharacter>(GetOwningPlayerPawn());

    if(character)
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

TArray<FString> UCustomRobotRebellionUserWidget::cooldownParseToScreen() const
{
    // get every actual cooldown - Maybe replicate cooldown on client!
    APlayableCharacter* character = Cast<APlayableCharacter>(GetOwningPlayerPawn());
    TArray<FString> cooldownsStr{};
    if(character->Role != ROLE_Authority)
    {
        int bp = 5;
        bp++;
    }
    else
    {
        int bp = 5;
        bp++;
    }

    if(character)
    { // Try to reach spell kit comp
        TArray<USpellKit*> spellKitComps;
        character->GetComponents<USpellKit>(spellKitComps);
        if(spellKitComps.Num() > 0)
        {
            TArray<float> cooldownsF = spellKitComps[0]->getCooldowns();
            for(int index{}; index < cooldownsF.Num(); ++index)
            {
                cooldownsStr.Emplace(processFloatCooldown(cooldownsF[index]));
            }
        }
    }
    return cooldownsStr;
}


FString UCustomRobotRebellionUserWidget::processFloatCooldown(float value) const
{
    if(value > 0.f)
    {
        if(value >= 1.1f)
        {
            // Only print second
            int32 valueInt = UKismetMathLibrary::FTrunc(value + 1.f);
            return FString::FromInt(valueInt);
        }
        else if(value >= 1.f)
        {// Only print 1
            return FString{"1"};
        }
        else
        {
            // print second value
            int32 valueInt = UKismetMathLibrary::FTrunc(value * 10.f);
            return FString::SanitizeFloat(static_cast<float>(valueInt) / 10.f);
        }
    }
    else
    {
        return FString{};
    }
}


FString UCustomRobotRebellionUserWidget::bombParseToScreen() const
{
    APlayableCharacter* character = Cast<APlayableCharacter>(GetOwningPlayerPawn());
    if(character)
    {
        if(character->m_bombCount == character->m_nbBombMax)
        {
            return FString{"MAX"};
        }
        else
        {
            return FString::FromInt(character->m_bombCount);
        }
    }
    else
    {
        return FString{};
    }
}

FString UCustomRobotRebellionUserWidget::healthPotsParseToScreen() const
{
    APlayableCharacter* character = Cast<APlayableCharacter>(GetOwningPlayerPawn());
    if(character)
    {
        if(character->m_healthPotionsCount == character->m_nbHealthPotionMax)
        {
            return FString{"MAX"};
        }
        else
        {
            return FString::FromInt(character->m_healthPotionsCount);
        }
    }
    else
    {
        return FString{};
    }
}

FString UCustomRobotRebellionUserWidget::manaPotsParseToScreen() const
{
    APlayableCharacter* character = Cast<APlayableCharacter>(GetOwningPlayerPawn());
    if(character)
    {
        if(character->m_manaPotionsCount == character->m_nbManaPotionMax)
        {
            return FString{"MAX"};
        }
        else
        {
            return FString::FromInt(character->m_manaPotionsCount);
        }
    }
    return FString{};
}

bool UCustomRobotRebellionUserWidget::isMainWeaponEquipped()const
{
    APlayableCharacter* character = Cast<APlayableCharacter>(GetOwningPlayerPawn());
    if(character)
    {
        return character->m_weaponInventory->isMainWeaponEquipped();
    }
    return true;
}

