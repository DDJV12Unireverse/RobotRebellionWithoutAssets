// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GraphHandler.generated.h"

UCLASS()
class ROBOTREBELLION_API AGraphHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraphHandler();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to clear the graph at the end of the game
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
