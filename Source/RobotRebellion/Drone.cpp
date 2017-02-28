// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Drone.h"


ADrone::ADrone() : ANonPlayableCharacter()
{
    this->setImmortal(true);

    this->GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Drone");
    this->GetCapsuleComponent()->SetEnableGravity(false); //the drone fly
}

