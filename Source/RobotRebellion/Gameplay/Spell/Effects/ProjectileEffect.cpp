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
    if(m_hasEffect)
    {
        spawnEffect();
    }

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

void AProjectileEffect::spawnEffect()
{
    m_particleSystemComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_particleSystem,
                                                                    GetActorLocation() + m_effectOffset, GetActorRotation(), true);
    m_particleSystemComp->SetRelativeScale3D(m_effectScale);
    if(Role >= ROLE_Authority)
    {
        multiSpawnEffect(GetActorLocation());
    }
}

void AProjectileEffect::multiSpawnEffect_Implementation(FVector location)
{
    m_particleSystemComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_particleSystem,
                                                                    location + m_effectOffset, GetActorRotation(), true);
    m_particleSystemComp->SetRelativeScale3D(m_effectScale);
}

bool AProjectileEffect::multiSpawnEffect_Validate(FVector location)
{
    return true;
}