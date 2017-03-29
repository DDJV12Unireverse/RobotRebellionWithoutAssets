// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NonPlayableCharacter.h"
#include "Drone.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ADrone : public ANonPlayableCharacter
{
    GENERATED_BODY()

public:
    ADrone();

    virtual void Tick(float deltaTime) override;
};
