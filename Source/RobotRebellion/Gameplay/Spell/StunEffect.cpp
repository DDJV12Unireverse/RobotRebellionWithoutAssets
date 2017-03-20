// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "StunEffect.h"

#include "../../Character/RobotRebellionCharacter.h"


void UStunEffect::BeginPlay()
{
    Super::BeginPlay();
}

void UStunEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStunEffect::exec(ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
{
    target->inflictStun(m_duration);
}

void UStunEffect::exec(const FVector impactPoint)
{
}
