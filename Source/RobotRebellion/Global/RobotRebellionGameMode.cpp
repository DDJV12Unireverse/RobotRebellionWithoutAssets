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

	m_gameMode = ECurrentGameMode::AMBIENT;
	m_previousGameMode = ECurrentGameMode::NONE;
	m_bossIsDead = false;
}

void ARobotRebellionGameMode::BeginPlay()
{
	setupAudioComponents();

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
	for (int i = 0; i < data.m_playableCharacterArray.Num(); i++)
	{
		APlayableCharacter* playableCharacter = data.m_playableCharacterArray[i];
		// PLAY COMBAT SOUND
		if (playableCharacter->m_isInCombat && (m_gameMode != ECurrentGameMode::BOSS || m_bossIsDead))
		{
			m_gameMode = ECurrentGameMode::COMBAT;
		}
		else if (m_gameMode != ECurrentGameMode::BOSS || m_bossIsDead)
		{
			m_gameMode = ECurrentGameMode::AMBIENT;
		}
		// TODO Boss Dead: Win GAME
	}

	if (m_gameMode != m_previousGameMode)
	{
		AudioManager& audioMan = AudioManager::getInstance();
		switch (m_gameMode)
		{
			case ECurrentGameMode::NONE:
			case ECurrentGameMode::AMBIENT:
				audioMan.playBackgroundMusic(m_ambientAudioComp);
				break;
			case ECurrentGameMode::COMBAT:
				audioMan.playBackgroundMusic(m_combatAudioComp);
				break;
			case ECurrentGameMode::BOSS:
				audioMan.playBackgroundMusic(m_bossAudioComp);
				break;
			case ECurrentGameMode::WIN:
				audioMan.playBackgroundMusic(m_winAudioComp);
				break;
			case ECurrentGameMode::LOSE:
				audioMan.playBackgroundMusic(m_loseAudioComp);
				break;
		}
	}

	m_previousGameMode = m_gameMode;
}

void ARobotRebellionGameMode::setBossGameMode()
{
	m_gameMode = ECurrentGameMode::BOSS;
}

void ARobotRebellionGameMode::setBossDead()
{
	m_bossIsDead = true;
}

void ARobotRebellionGameMode::setupAudioComponents()
{

	if (m_ambientSounds && !m_ambientAudioComp)
	{
		m_ambientAudioComp = NewObject<UAudioComponent>(this);
		m_ambientAudioComp->SetSound(m_ambientSounds);
	}
	if (m_combatSounds && !m_combatAudioComp)
	{
		m_combatAudioComp = NewObject<UAudioComponent>(this);
		m_combatAudioComp->SetSound(m_combatSounds);
	}
	if (m_bossSounds && !m_bossAudioComp)
	{
		m_bossAudioComp = NewObject<UAudioComponent>(this);
		m_bossAudioComp->SetSound(m_bossSounds);
	}
	if (m_winSounds && !m_winAudioComp)
	{
		m_winAudioComp = NewObject<UAudioComponent>(this);
		m_winAudioComp->SetSound(m_winSounds);
	}
	if (m_loseSounds && !m_loseAudioComp)
	{
		m_loseAudioComp = NewObject<UAudioComponent>(this);
		m_loseAudioComp->SetSound(m_loseSounds);
	}
}
