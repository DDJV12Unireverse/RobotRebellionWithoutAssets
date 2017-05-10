// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "KingActivateTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API AKingActivateTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
	
public:
    AKingActivateTriggerBox();

    virtual void BeginPlay() override;

    UFUNCTION()
        void onEnter(UPrimitiveComponent* var1, AActor* enteredActor, UPrimitiveComponent* var3, int32 var4, bool var5, const FHitResult& var6);


private:
    void internal_signalToServer();

    void correctDestruction();

    void killItself();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverKills();
        
    UFUNCTION(Reliable, NetMulticast)
        void multiKills();

    UFUNCTION(Reliable, Server, WithValidation)
        void signalToServer();
};
