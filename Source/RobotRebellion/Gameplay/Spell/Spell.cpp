// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Spell.h"

#include "../../Tool/UtilitaryFunctionLibrary.h"
#include "Gameplay/Spell/Effects/Effect.h"
#include "Character/RobotRebellionCharacter.h"


USpell::USpell()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USpell::BeginPlay()
{
    Super::BeginPlay();
}

void USpell::cast()
{}

void USpell::initializeSpell()
{
    for(int i = 0; i < m_effectsClass.Num(); ++i)
    {
        UEffect* tempEffect;

        tempEffect = NewObject<UEffect>(this, m_effectsClass[i]);

        if(tempEffect)
        {
            m_effects.Emplace(tempEffect);
        }
    }
}

bool USpell::canCast() const
{
    return (FPlatformTime::Seconds() > m_nextAllowedCastTimer)
        && Cast<ARobotRebellionCharacter>(GetOwner())->getMana() >= m_manaCost;
}

float USpell::getCurrentCooldown()const
{
    if(!canCast())
    {
        return m_nextAllowedCastTimer - FPlatformTime::Seconds();
    }
    else
    {
        return -1.0f;
    }
}