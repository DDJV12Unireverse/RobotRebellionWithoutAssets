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
        void onHit(UPrimitiveComponent* var1, AActor* var2, UPrimitiveComponent* var3, FVector var4, const FHitResult& var5);


private:
    void correctDestruction();

    void killItself();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverKills();
        
    UFUNCTION(Reliable, NetMulticast)
        void multiKills();
};
