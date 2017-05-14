// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "BigRoomTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ABigRoomTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:

    ABigRoomTriggerBox();

    virtual void BeginPlay() override;

    UFUNCTION()
        void onOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                            bool bFromSweep, const FHitResult& SweepResult);

    
	
	
};