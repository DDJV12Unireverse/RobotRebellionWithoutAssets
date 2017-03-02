// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Spell.h"

#include "UtilitaryFunctionLibrary.h"
#include "Effect.h"

USpell::USpell()
{
    // TODO
}

void USpell::BeginPlay()
{
    Super::BeginPlay();

    for(int i = 0; i < m_effectsClass.Num(); ++i)
    {
        UEffect* tempEffect;

        UUtilitaryFunctionLibrary::createObjectFromDefault<UEffect>(
            &tempEffect,
            m_effectsClass[i],
            this,
            TEXT("Effect")
            );

        if(tempEffect)
        {
            m_effects.Emplace(tempEffect);
        }
    }
}
