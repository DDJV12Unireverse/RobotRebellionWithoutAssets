// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "BossRobot.h"
#include "Global/RobotRebellionGameMode.h"

ABossRobot::ABossRobot()
{
    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Robots");

    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}

void ABossRobot::cppOnDeath()
{
    UWorld* w = this->GetWorld();
    ARobotRebellionGameMode* gameMode = Cast<ARobotRebellionGameMode>(w->GetAuthGameMode());
    if(gameMode)
    {
        gameMode->setBossDead();
    }
    Super::cppOnDeath();
}