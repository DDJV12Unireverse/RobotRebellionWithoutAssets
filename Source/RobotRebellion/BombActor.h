// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PickupActor.h"
#include "BombActor.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ABombActor : public APickupActor
{
	GENERATED_BODY()
	
	
public:
    void OnPickup(APawn* InstigatorPawn) override;

    virtual EObjectType getObjectType() const USE_NOEXCEPT override
    {
        return EObjectType::BOMB;
    }
};
