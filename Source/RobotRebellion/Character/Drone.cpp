// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Drone.h"
#include "Gameplay/Weapon/Kaboom.h"
#include "Tool/UtilitaryFunctionLibrary.h"
#include "Components/SplineComponent.h"


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

}


void ADrone::displayScore(float scores[5])
{
    if (this->getBillboardComponent())
    {
        this->displayAnimatedText(FString::Printf(TEXT("Follow :%f \n Reload :%f \n Attack :%f \n Drop :%f \n Wait :%f"),
            scores[0], scores[1], scores[2], scores[3], scores[4]), FColor::Blue, ELivingTextAnimMode::TEXT_ANIM_NOT_MOVING);

    }
}



bool ADrone::reload()
{
    if(Role < ROLE_Authority)
    {
        return false;
    }

    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Silver, "Loading Bomb");

    UWorld* world = this->GetWorld();

    if(!this->isLoaded() && world)
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

        if(m_currentBomb)
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
    if(Role < ROLE_Authority)
    {
        return;
    }

    if(this->isLoaded())
    {
        m_currentBomb->activateBomb();

        m_currentBomb->detachFromDrone();

        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Silver, "DROP");

        m_currentBomb = nullptr;
    }
}

void ADrone::autoDrop(float deltaTime)
{
    m_debugTimer += deltaTime;

    if(m_debugTimer > m_debugAutoDropTimer)
    {
        drop();

        m_debugTimer = 0.f;
    }
}

float ADrone::getBombBaseDamage() const USE_NOEXCEPT
{
    return m_currentBomb->m_baseDamage;
}

float ADrone::getBombRadius() const USE_NOEXCEPT
{
    return m_currentBomb->m_detonationRadius;
}