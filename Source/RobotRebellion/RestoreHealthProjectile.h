// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Weapon/Projectile.h"
#include "RestoreHealthProjectile.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API ARestoreHealthProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
        float m_restoredHealth;

public:
    ARestoreHealthProjectile();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;

    // On hit function called every collision
    UFUNCTION()
        virtual void OnHit(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent*
                           OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
};
