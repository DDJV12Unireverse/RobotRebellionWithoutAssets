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
    // TODO
}

void USpawnEffect::exec(const FVector impactPoint)
{
    // TODO - Spawn the actor!
}