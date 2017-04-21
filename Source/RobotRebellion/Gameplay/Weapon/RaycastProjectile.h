// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Weapon/Projectile.h"
#include "RaycastProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ARaycastProjectile : public AProjectile
{
	GENERATED_BODY()
	
	
public:
    FORCEINLINE virtual bool isRaycast() const USE_NOEXCEPT override
    {
        return true;
    }
};
