// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Weapon/Projectile.h"
#include "ProjectileEffect.generated.h"

/**
 *
 */
UCLASS()
class ROBOTREBELLION_API AProjectileEffect : public AProjectile
{
    GENERATED_BODY()
private:
    class UThrowSpell* m_parentSpell;

public:
    AProjectileEffect();

    void initMovement(const FVector& shootDirection);

    void setParent(UThrowSpell *spell);

    virtual void OnHit(UPrimitiveComponent* ThisComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
    
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
};
