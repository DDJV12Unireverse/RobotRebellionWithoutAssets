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
    bool m_isCrouch;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        ULootTable* m_lootTable;
public:
    ANonPlayableCharacter();
    //Loot Probability

    void dropLoot();
    UFUNCTION(Server, Reliable, WithValidation)
        void serverDropLoot();
    virtual void cppOnDeath() override;

    virtual FVector aim(const FVector& directionToShoot) const override;
};
