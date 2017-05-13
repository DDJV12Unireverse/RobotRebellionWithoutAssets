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
    UFUNCTION(Reliable, Client)
        void setInputMode(bool status);
  
    void BeginPlay() override;
    void Tick(float deltaTime) override;
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
    

};
