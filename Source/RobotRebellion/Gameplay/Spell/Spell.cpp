// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Spell.h"

#include "../../Tool/UtilitaryFunctionLibrary.h"
#include "Gameplay/Spell/Effects/Effect.h"
#include "Character/RobotRebellionCharacter.h"


USpell::USpell()
{
    PrimaryComponentTick.bCanEverTick = true;
    bReplicates = true;
}

void USpell::BeginPlay()
{
    Super::BeginPlay();
}


void USpell::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    if(GetOwner()->Role == ROLE_Authority)
    {
        if(!canCast())
        {
            m_currentCooldown = m_nextAllowedCastTimer - FPlatformTime::Seconds();
        }
        else
        {
            m_currentCooldown = -1.f;
        }
    }
}

void USpell::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(USpell, m_currentCooldown, COND_OwnerOnly);
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
    this->RegisterComponent();
}

bool USpell::canCast() const
{
    return (FPlatformTime::Seconds() > m_nextAllowedCastTimer)
        && Cast<ARobotRebellionCharacter>(GetOwner())->getMana() >= m_manaCost;
}

float USpell::getCurrentCooldown() const
{
    return m_currentCooldown;
}