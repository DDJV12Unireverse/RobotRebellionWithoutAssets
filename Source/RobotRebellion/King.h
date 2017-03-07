// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NonPlayableCharacter.h"
#include "King.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API AKing : public ANonPlayableCharacter
{
	GENERATED_BODY()
	
public:
    AKing();


    virtual void Tick(float deltaTime) override;
	
	
};
