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
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Debug")
    class UWidgetComponent* m_utScores;

    ADrone();
    virtual void BeginPlay() override;
    virtual void Tick(float deltaTime) override;

    void displayScore(float scores[5]);
    
};
