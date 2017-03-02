// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "NonPlayableCharacter.h"

ANonPlayableCharacter::ANonPlayableCharacter() : ARobotRebellionCharacter()
{
    // fill it
    m_lootTable = CreateDefaultSubobject<ULootTable>(TEXT("LootTable"));
}


void ANonPlayableCharacter::cppOnDeath()
{
    dropLoot();
    this->Destroy();
}

void ANonPlayableCharacter::dropLoot()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Black, "Drop!");
    }
    m_lootTable->dropItem(GetActorLocation());
}