// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "InvisibilityEffect.h"

#include "Character/RobotRebellionCharacter.h"


void UInvisibilityEffect::BeginPlay()
{
    Super::BeginPlay();
}

void UInvisibilityEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInvisibilityEffect::exec(ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
{
    caster->inflictInvisibility();
}

void UInvisibilityEffect::exec(const FVector impactPoint, ARobotRebellionCharacter* caster)
{
}
