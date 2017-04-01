// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Weapon/Projectile.h"
#include "FireProjectile.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API AFireProjectile : public AProjectile
{
	GENERATED_BODY()
	
	public:
        UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Projectile Settings")
        float m_baseDamage;

        AFireProjectile();

        virtual void BeginPlay();

        virtual void Tick(float DeltaTime);

        UFUNCTION()
            virtual void OnHit(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent*
                OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	
};
