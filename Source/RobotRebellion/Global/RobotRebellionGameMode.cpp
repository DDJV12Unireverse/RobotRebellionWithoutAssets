// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RobotRebellion.h"
#include "RobotRebellionGameMode.h"
#include "Character/RobotRebellionCharacter.h"

#include "GameInstaller.h"
#include "Gameplay/Alteration/StunAlteration.h"
#include "Gameplay/Alteration/InvisibilityAlteration.h"

ARobotRebellionGameMode::ARobotRebellionGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> defaultPawn(TEXT("/Game/ThirdPersonCPP/Blueprints/DefaultSpawn_BP"));
    DefaultPawnClass = defaultPawn.Class;
    bUseSeamlessTravel = true;
}

void ARobotRebellionGameMode::BeginPlay()
{
    Super::BeginPlay();

    GameAlterationInstaller::getInstance().installAlteration<UStunAlteration>(&m_stunDefault);
    GameAlterationInstaller::getInstance().installAlteration<UInvisibilityAlteration>(&m_invisibleDefault);
}