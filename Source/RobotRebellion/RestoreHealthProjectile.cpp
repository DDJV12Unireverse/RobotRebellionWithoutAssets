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

void ARestoreHealthProjectile::OnHit(UPrimitiveComponent * ThisComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
    if(Role == ROLE_Authority)
    {
        ARobotRebellionCharacter* receiver = Cast<ARobotRebellionCharacter>(OtherActor);
        if(receiver && m_owner != receiver && !receiver->isDead())
        {
            if(!receiver->isImmortal())
            {
                receiver->restoreHealth(m_restoredHealth);
            }
            else
            {
                receiver->displayAnimatedText("IMMORTAL OBJECT", FColor::Purple, ELivingTextAnimMode::TEXT_ANIM_NOT_MOVING);
            }
        }

        Destroy();
    }
}
