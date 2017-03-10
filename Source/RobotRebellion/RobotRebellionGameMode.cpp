// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RobotRebellion.h"
#include "RobotRebellionGameMode.h"
#include "RobotRebellionCharacter.h"

#include "GameInstaller.h"

ARobotRebellionGameMode::ARobotRebellionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/Soldier_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ARobotRebellionGameMode::BeginPlay()
{
    Super::BeginPlay();

    GameAlterationInstaller::getInstance().installAlteration<UStunAlteration>(&m_stunDefault);
    GameAlterationInstaller::getInstance().installAlteration<UInvisibilityAlteration>(&m_invisibleDefault);
}