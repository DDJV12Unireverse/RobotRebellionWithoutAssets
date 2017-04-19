// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.


#include "RobotRebellion.h"
#include "AudioManager.h"
#include "RobotRebellionGameMode.h"
#include "Character/RobotRebellionCharacter.h"

#include "GameInstaller.h"
#include "Gameplay/Alteration/StunAlteration.h"
#include "Gameplay/Alteration/InvisibilityAlteration.h"

#include "EntityDataSingleton.h"
#include "IA/Navigation/NavigationVolumeGraph.h"
#include "Tool/UtilitaryMacros.h"


ARobotRebellionGameMode::ARobotRebellionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> defaultPawn(TEXT("/Game/ThirdPersonCPP/Blueprints/DefaultSpawn_BP"));
	DefaultPawnClass = defaultPawn.Class;
	bUseSeamlessTravel = true;

	PrimaryActorTick.bCanEverTick = true;

    //GameStateClass = ARobotRebellionGameState::StaticClass();
}

void ARobotRebellionGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void ARobotRebellionGameMode::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}

