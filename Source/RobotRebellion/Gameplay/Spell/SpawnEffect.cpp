// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "SpawnEffect.h"


void USpawnEffect::BeginPlay()
{
    Super::BeginPlay();
}

void USpawnEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpawnEffect::exec(class ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
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

void USpawnEffect::exec(const FVector impactPoint)
{
    // Not implemented in this Effect
}