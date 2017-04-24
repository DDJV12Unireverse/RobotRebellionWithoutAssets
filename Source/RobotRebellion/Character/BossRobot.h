// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/NonPlayableCharacter.h"
#include "BossRobot.generated.h"

/**
 *
 */
UCLASS()
class ROBOTREBELLION_API ABossRobot : public ANonPlayableCharacter
{
    GENERATED_BODY()


public:
    ABossRobot();

    virtual void BeginPlay() override;

    virtual void cppOnDeath() override;
};
