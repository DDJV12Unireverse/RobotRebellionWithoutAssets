// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Weapon/Projectile.h"
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particule)
        UParticleSystem* m_particleSystem;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particule)
        UParticleSystemComponent* m_particleSystemComp;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particule)
        bool m_hasEffect;


public:
    AProjectileEffect();

    void initMovement(const FVector& shootDirection);

    void setParent(UThrowSpell *spell);

    virtual void OnHit(UPrimitiveComponent* ThisComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
    
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    void spawnEffect();

public:
    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiSpawnEffect(FVector location);
};

