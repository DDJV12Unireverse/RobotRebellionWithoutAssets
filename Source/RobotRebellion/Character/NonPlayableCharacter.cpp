// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "NonPlayableCharacter.h"

#include "UI/TextBillboardComponent.h"
#include "IA/Controller/CustomAIControllerBase.h"

ANonPlayableCharacter::ANonPlayableCharacter() : ARobotRebellionCharacter()
{
    // fill it
    m_lootTable = CreateDefaultSubobject<ULootTable>(TEXT("LootTable"));
}


void ANonPlayableCharacter::cppOnDeath()
{
    dropLoot();
    this->m_alterationController->removeAllAlteration();

    ACustomAIControllerBase* controller = Cast<ACustomAIControllerBase>(this->GetController());
    if (controller)
    {
        controller->setTarget(nullptr);
    }

    this->startTimedDestroy();
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

FVector ANonPlayableCharacter::aim(const FVector& directionToShoot) const
{
    constexpr float fallOffAngle = 0.12f;

    FVector result = directionToShoot;
    result.Y += FMath::RandRange(-fallOffAngle, fallOffAngle);
    result.Z += FMath::RandRange(-fallOffAngle, fallOffAngle);

    result.Normalize();

    return result;
}