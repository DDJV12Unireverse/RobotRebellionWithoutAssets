// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RobotRebellionCharacter.h"
#include "../Global/LootTable.h"
#include "NonPlayableCharacter.generated.h"

/**
 * Mother class for every npc in RobotRebellion Game
 */
UCLASS()
class ROBOTREBELLION_API ANonPlayableCharacter : public ARobotRebellionCharacter
{
	GENERATED_BODY()
       
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
        bool m_isCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        ULootTable* m_lootTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Effect")
        UParticleSystem* m_spawnParticleSystem;


public:
    ANonPlayableCharacter();

    ////Server
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    void spawnEffect();

    virtual void cppOnDeath() override;

    virtual FVector aim(const FVector& directionToShoot) const override;

    void goAway(const FVector& fromWhere, float delta);

    //Loot Probability
    void dropLoot();

    UFUNCTION(Server, Reliable, WithValidation)
        void serverDropLoot();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiSpawnEffect();
};
