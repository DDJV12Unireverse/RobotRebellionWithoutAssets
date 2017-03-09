// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "King.h"
#include "../IA/DroneAIController.h"


AKing::AKing() : ANonPlayableCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    this->setImmortal(false);

    this->GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("King");
    this->GetCharacterMovement()->GravityScale = 0.f;
}

void AKing::BeginPlay()
{
    Super::BeginPlay();
    TArray<AActor*> drones;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), m_droneControllerClass, drones);
    if (drones.Num() > 0) //The king is here
    {
        ADroneAIController* drone = static_cast<ADroneAIController*>(drones.Top());
        drone->setFollowKing(); //king is spawned, follow him.

    }
}

void AKing::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}

void AKing::cppOnDeath()
{
    TArray<AActor*> drones;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), m_droneControllerClass, drones);

    ADroneAIController* drone = static_cast<ADroneAIController*>(drones.Top());
    drone->setFollowGroup(); //king is dead, follow group. Later -> Game over.


    Super::cppOnDeath();
}