// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
        TSubclassOf<class UUserWidget> m_HUDWidgets;

    // Variable to hold the widgets from m_HUDWidgets after creating it.
    UUserWidget* m_HUDContainer;


public:
    void initializeHUD();
};
