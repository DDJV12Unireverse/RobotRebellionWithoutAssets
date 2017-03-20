// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DamageZone.generated.h"

UCLASS()
class ROBOTREBELLION_API ADamageZone : public AActor
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageZone)
        /** radius to define the damage zone*/
        float m_radius;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageZone)
        /** define how many damage will be deal to the actors (damage will be reduced)*/
        float m_damagePerTick;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageZone)
        /** how many time the zone should inflict damage in one second*/
        float m_tickRate;

private:
    // Private float to avoid doing the division every tick
    float m_secondBetweenTick;
    // Store how many long ago was the last tick
    float m_deltaSinceLastTick;

public:	
	// Sets default values for this actor's properties
	ADamageZone();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
