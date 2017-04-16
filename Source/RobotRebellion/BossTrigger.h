// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BossTrigger.generated.h"

UCLASS()
class ROBOTREBELLION_API ABossTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
    UPROPERTY(EditAnywhere, Category = "TriggerZone")
        UBoxComponent* m_bossTriggerBox;

	// Sets default values for this actor's properties
	ABossTrigger();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
        void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &
                            SweepResult);
	
};
