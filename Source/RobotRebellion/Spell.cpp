// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Spell.h"

#include "UtilitaryFunctionLibrary.h"
#include "Effect.h"
#include "RobotRebellion.h"

USpell::USpell()
{
    // TODO
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

bool USpell::canCast()
{
    return (FPlatformTime::Seconds() > m_nextAllowedCastTimer) 
        && Cast<ARobotRebellionCharacter>(GetOwner())->getMana() > m_manaCost;
}