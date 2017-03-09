// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "DamageEffect.h"

#include "Damage.h"
#include "../Tool/Algorithm.h"
#include "../Character/RobotRebellionCharacter.h"

void UDamageEffect::BeginPlay()
{
    Super::BeginPlay();
}

void UDamageEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDamageEffect::exec(class ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
{
    // Very simple way to deals damage
    float damage = m_flatDamage;
    damage += target->getMaxHealth() * m_hpPercent;
    float coef = caster->getStrength() / target->getDefense();
    coef = coef > 1.0f ? 1.0f : coef;
    damage += coef * m_reducedDamage;

    target->inflictDamage(damage);
    
    if(target->isDead())
    {
        target->onDeath();
    }
}

void UDamageEffect::exec(const FVector impactPoint)
{
    // Not implemented in this Effect
}

