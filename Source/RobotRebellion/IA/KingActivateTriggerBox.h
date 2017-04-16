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

    UFUNCTION()
        void onHit(UPrimitiveComponent* var1, AActor* var2, UPrimitiveComponent* var3, FVector var4, const FHitResult& var5);
};
