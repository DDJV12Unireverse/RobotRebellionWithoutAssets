// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "BossRobot.h"
#include "Global/WorldInstanceEntity.h"

ABossRobot::ABossRobot()
{
    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Robots");

    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
}

void ABossRobot::BeginPlay()
{
    Super::BeginPlay();

    UWorld* world = this->GetWorld();
    TArray<AActor*> entity;
    UGameplayStatics::GetAllActorsOfClass(world, AWorldInstanceEntity::StaticClass(), entity);
    if(entity.Num() > 0)
    {
        Cast<AWorldInstanceEntity>(entity[0])->setBossGameMode();
    }
}

void ABossRobot::cppOnDeath()
{
    UWorld* w = this->GetWorld();
    TArray<AActor*> entity;
    UGameplayStatics::GetAllActorsOfClass(w, AWorldInstanceEntity::StaticClass(), entity);
    if(entity.Num() > 0)
    {
        Cast<AWorldInstanceEntity>(entity[0])->setBossDead();
    }
    Super::cppOnDeath();
}