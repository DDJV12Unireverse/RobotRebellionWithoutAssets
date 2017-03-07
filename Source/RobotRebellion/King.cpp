// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "King.h"


AKing::AKing() : ANonPlayableCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    this->setImmortal(false);

    this->GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("King");
    this->GetCharacterMovement()->GravityScale = 0.f;
}

void AKing::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}

