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

void USpawnEffect::exec(ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
{
    // TODO
}

void USpawnEffect::exec(const FVector impactPoint)
{
    // TODO - Spawn the actor!
    // update position
    FVector spawnLocation = impactPoint + m_offsetFromImpactPoint;
    GetWorld()->SpawnActor<AActor>(m_actorClassToSpawn, spawnLocation, FRotator{0.f});
}