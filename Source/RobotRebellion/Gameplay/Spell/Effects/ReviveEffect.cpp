// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ReviveEffect.h"

#include "Character/RobotRebellionCharacter.h"
#include "Character/PlayableCharacter.h"


void UReviveEffect::BeginPlay()
{
    Super::BeginPlay();
}

void UReviveEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UReviveEffect::exec(ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
{
    APlayableCharacter* playableCaster = Cast<APlayableCharacter>(caster);
    APlayableCharacter* playableTarget = Cast<APlayableCharacter>(target);
    if(playableCaster && playableTarget)
    {
        playableCaster->cppPreRevive(playableTarget);
        playableTarget->spawnReviveParticle();
    }
}

void UReviveEffect::exec(const FVector& impactPoint, ARobotRebellionCharacter* caster)
{}