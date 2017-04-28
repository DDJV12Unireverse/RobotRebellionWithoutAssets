// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DamageZone.generated.h"

UCLASS()
class ROBOTREBELLION_API ADamageZone : public AActor
{
    GENERATED_BODY()

public:
    /** radius to define the damage zone*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageZone)
        float m_radius;
    /** define how many damage will be deal to the actors (damage will be reduced)*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageZone)
        float m_damagePerTick;
    /** how many time the zone should inflict damage in one second*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageZone)
        float m_tickRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageZone)
        bool m_isMolotov;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageZone)
        TArray<TEnumAsByte<EObjectTypeQuery>> m_objectTypes;

private:
    // Private float to avoid doing the division every tick
    float m_secondBetweenTick;
    // Store how many long ago was the last tick
    float m_deltaSinceLastTick;
    int m_burnedActors;

public:
    // Sets default values for this actor's properties
    ADamageZone();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;



};
