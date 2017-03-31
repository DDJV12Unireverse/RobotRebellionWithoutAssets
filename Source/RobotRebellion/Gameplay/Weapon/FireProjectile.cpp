// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "FireProjectile.h"
#include "Character/RobotRebellionCharacter.h"


AFireProjectile::AFireProjectile()
{

}


void AFireProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void AFireProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFireProjectile::OnHit(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (Role == ROLE_Authority)
    {
        ARobotRebellionCharacter* receiver = Cast<ARobotRebellionCharacter>(OtherActor);
        if (receiver && m_owner != receiver && !receiver->isDead())
        {
            if (!receiver->isImmortal())
            {
                receiver->inflictDamage(m_baseDamage);
            }
            else
            {
                receiver->displayAnimatedText("IMMORTAL OBJECT", FColor::Purple, ELivingTextAnimMode::TEXT_ANIM_NOT_MOVING);
            }
        }

        //TODO display burn effect on Mesh

        Destroy();
    }
}