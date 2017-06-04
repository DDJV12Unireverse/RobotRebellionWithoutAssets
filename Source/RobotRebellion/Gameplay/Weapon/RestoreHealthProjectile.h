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
	
    virtual void inflictDamageLogic(class AActor* OtherActor, const FHitResult& Hit) override;

    FORCEINLINE virtual bool isRaycast() const USE_NOEXCEPT
    {
        return true;
    }
};
