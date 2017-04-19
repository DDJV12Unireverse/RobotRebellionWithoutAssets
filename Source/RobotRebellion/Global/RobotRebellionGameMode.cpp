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

	GameAlterationInstaller& installer = GameAlterationInstaller::getInstance();
	installer.installAlteration<UStunAlteration>(&m_stunDefault);
	installer.installAlteration<UInvisibilityAlteration>(&m_invisibleDefault);
	installer.installAlteration<UShieldAlteration>(&m_shieldDefault);

	EntityDataSingleton& datas = EntityDataSingleton::getInstance();
	datas.m_showVolumeBox = this->m_showVolumeBox;
	datas.m_showEnnemyDetectionSphere = this->m_showEnnemyDetectionSphere;

	NavigationVolumeGraph& navGraph = NavigationVolumeGraph::getInstance();
	navGraph.m_showConnection = this->m_showVolumeConnection;
}

void ARobotRebellionGameMode::Tick(float deltaTime)
{

	EntityDataSingleton& data = EntityDataSingleton::getInstance();
	data.update(this->GetWorld());
   
}

