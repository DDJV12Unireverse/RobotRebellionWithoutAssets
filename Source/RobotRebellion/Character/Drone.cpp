// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Drone.h"

#include "Gameplay/Weapon/Kaboom.h"
#include "Tool/UtilitaryFunctionLibrary.h"


ADrone::ADrone() : ANonPlayableCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    this->setImmortal(true);

    this->GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Drone");
    this->GetCharacterMovement()->GravityScale = 0.f;

    m_debugTimer = 0.f;
}

void ADrone::BeginPlay()
{
    Super::BeginPlay();

    reload();
}

void ADrone::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    autoDrop(deltaTime);
}

bool ADrone::reload()
{
    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Silver, "Loading Bomb");

    UWorld* world = this->GetWorld();

    if (!this->isLoaded() && world)
    {
        FActorSpawnParameters spawnParams;
        spawnParams.Owner = this;
        spawnParams.Instigator = this->Instigator;

        m_currentBomb = world->SpawnActor<AKaboom>(
            m_defaultKaboomBomb,
            m_bombAccroch,
            { 0.f, 0.f, 0.f },
            spawnParams
        );

        if (m_currentBomb)
        {
            m_currentBomb->attachToDrone(this);

            PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Silver, "LOADED");

            return true;
        }
    }

    return false;
}

void ADrone::drop()
{
    if (this->isLoaded())
    {
        m_currentBomb->detachFromDrone();

        m_currentBomb->activateBomb();

        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Silver, "DROP");

        m_currentBomb = nullptr;
    }
}

void ADrone::autoDrop(float deltaTime)
{
    m_debugTimer += deltaTime;

    if (m_debugTimer > m_debugAutoDropTimer)
    {
        drop();

        m_debugTimer = 0.f;
    }
}
