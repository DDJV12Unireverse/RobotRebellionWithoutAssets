// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RobotRebellionWidget.h"
#include "TopWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UTopWidget : public URobotRebellionWidget
{
	GENERATED_BODY()
public:
	
    UFUNCTION(BlueprintCallable, Category = "Menu")
        void SinglePlayerGame();

    UFUNCTION(BlueprintCallable, Category = "Menu")
        void CloseSinglePlayerGameWidget();

    UFUNCTION(BlueprintCallable, Category = "Menu")
        void NetworkPlayerGame();

    UFUNCTION(BlueprintCallable, Category = "Menu")
        void GameOptionsMenu();

    UFUNCTION(BlueprintCallable, Category = "Menu")
        void CloseGameOptionsMenu();

};
