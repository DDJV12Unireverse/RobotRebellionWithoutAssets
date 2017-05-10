// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RestoreHealthProjectile.h"

ARestoreHealthProjectile::ARestoreHealthProjectile() : AProjectile()
{}

void ARestoreHealthProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void ARestoreHealthProjectile::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ARestoreHealthProjectile::inflictDamageLogic(class AActor* otherActor, const FHitResult& hit)
{
    ARobotRebellionCharacter* receiver = Cast<ARobotRebellionCharacter>(otherActor);
    if(receiver && m_owner != receiver && !receiver->isDead() && !receiver->isImmortal())
    {
         receiver->restoreHealth(m_restoredHealth);
    }
}