// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Spell/Effect.h"
#include "SpawnEffect.generated.h"

/**
 * This is an Effect that can be added to a spell.
 * This effect spawn a new AActor at the impact point + an offset
 * you can set
 *   the offset, 
 *   the base speed of the actor // the actor must have a projectileMovement component
 *   If it has lifetime
 *   the duration if it has life time
 *   if the ator should be controlled by AI controller -seems incompatible with initial speed :)
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API USpawnEffect : public UEffect
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnEffect)
        TSubclassOf<AActor> m_actorClassToSpawn;
    /** the actor will be spawned at the hit point translated by this offset*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnEffect)
        FVector m_offsetFromImpactPoint;
    /** Initiale speed, include the speed value not only the direction. The actor must have projectileMovementComponent to work*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnEffect)
        FVector  m_startSpeed;
    /** Maximal speed*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnEffect)
        float m_MaxSpeed;
    /** Enabled spawning default controller*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnEffect)
        bool m_hasDefaultAIController;
    /** set the life time in second*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnEffect)
        float m_actorLifeTime;
	

public:
    // Called when the game starts
    virtual void BeginPlay() override;
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // The behavior of the effect when it's a targeted effect
    virtual void exec(class ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target) override;
    // The behavior of the effect when it's point effect
    virtual void exec(const FVector impactPoint) override;
};
