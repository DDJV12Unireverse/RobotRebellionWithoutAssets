// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ProjectileEffect.h"

#include "Gameplay/Spell/ThrowSpell.h"


AProjectileEffect::AProjectileEffect() : AProjectile()
{
    m_collisionComp->OnComponentHit.Clear();
    m_collisionComp->OnComponentHit.AddDynamic(this, &AProjectileEffect::OnHit);
}

void AProjectileEffect::initMovement(const FVector& shootDirection)
{
    if(m_projectileMovement)
    {
        // Adjust velocity with direction
        m_projectileMovement->Velocity = shootDirection * m_projectileMovement->InitialSpeed;
    }
}

void AProjectileEffect::setParent(UThrowSpell* effect)
{
    m_parentSpell = effect;
}

void AProjectileEffect::OnHit(UPrimitiveComponent* ThisComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if(Role == ROLE_Authority)
    {
        m_parentSpell->onHit(ThisComp, OtherActor, OtherComp, NormalImpulse, Hit);
        Destroy();
    }
    else
    {
        Destroy();
    }
}

void AProjectileEffect::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    //DOREPLIFETIME(AProjectile, m_owner);
}
