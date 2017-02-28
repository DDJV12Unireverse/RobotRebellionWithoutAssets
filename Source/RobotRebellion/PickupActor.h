// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

UCLASS()
class ROBOTREBELLION_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    virtual void OnBeginFocus();
    virtual void OnEndFocus();
    virtual void OnPickup(APawn* InstigatorPawn);

    UPROPERTY(EditDefaultsOnly, Category = "Mesh")
        UStaticMeshComponent* MeshComp;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
        USoundCue* PickupSound;

};
