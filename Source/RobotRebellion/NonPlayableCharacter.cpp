// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "NonPlayableCharacter.h"

#include "TextBillboardComponent.h"

ANonPlayableCharacter::ANonPlayableCharacter() : ARobotRebellionCharacter()
{
    // fill it
    m_lootTable = CreateDefaultSubobject<ULootTable>(TEXT("LootTable"));
}


void ANonPlayableCharacter::cppOnDeath()
{
    if (this->m_textBillboardInstance)
    {
        this->m_textBillboardInstance->clearAllLivingTexts();
    }

    dropLoot();

    this->Destroy();
}

void ANonPlayableCharacter::dropLoot()
{
    if (Role == ROLE_Authority)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Black, "Drop!");
        m_lootTable->dropItem(GetActorLocation());
    }
    else
    {
        serverDropLoot();
    }
        
}
void ANonPlayableCharacter::serverDropLoot_Implementation()
{
    dropLoot();
}

bool ANonPlayableCharacter::serverDropLoot_Validate()
{
    return true;
}