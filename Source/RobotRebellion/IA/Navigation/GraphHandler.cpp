// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "GraphHandler.h"
#include "NavigationVolumeGraph.h"

// Sets default values
AGraphHandler::AGraphHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGraphHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGraphHandler::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGraphHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    NavigationVolumeGraph::getInstance().clearGraph();
}
