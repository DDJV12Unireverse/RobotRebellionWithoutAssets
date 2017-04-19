// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "BossRobot.h"
#include "Global/RobotRebellionGameMode.h"
#include "Global/RobotRebellionGameState.h"

ABossRobot::ABossRobot()
{
    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Robots");

    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}

void ABossRobot::cppOnDeath()
{
    UWorld* w = this->GetWorld();
    ARobotRebellionGameState* gameState = Cast<ARobotRebellionGameState>(w->GetGameState());
    if(gameState)
    {
        gameState->setBossDead();
    }
    Super::cppOnDeath();
}