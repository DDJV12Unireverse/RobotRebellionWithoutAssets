// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "DamageEffect.h"

#include "Damage.h"
#include "Algorithm.h"
#include "RobotRebellionCharacter.h"

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
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, "exec with target");
    float damage = m_flatDamage;
    damage += target->getMaxHealth() * m_hpPercent;
    //PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, "deal : "+ FString::FromInt(damage));
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

