// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "TrainingDummyCharacter.h"

ATrainingDummyCharacter::ATrainingDummyCharacter() : ANonPlayableCharacter()
{
    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Robots");
}