// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ObjectTypes.h"
#include "Focusable.h"
#include "PickupActor.generated.h"

UCLASS()
class ROBOTREBELLION_API APickupActor : public AActor, public Focusable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


    
    /* UFUNCTION(Reliable, Server, WithValidation)
         void serverOnPickup(APawn* InstigatorPawn);*/

    virtual EObjectType getObjectType() const USE_NOEXCEPT
    {
        return EObjectType::NONE;
    }
    UPROPERTY(EditDefaultsOnly, Category = "Mesh")
        UStaticMeshComponent* MeshComp;
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
        USoundCue* PickupSound;
    virtual void OnBeginFocus() override;
    virtual void OnEndFocus() override;
    virtual void OnPickup(APawn * InstigatorPawn) override;
};
