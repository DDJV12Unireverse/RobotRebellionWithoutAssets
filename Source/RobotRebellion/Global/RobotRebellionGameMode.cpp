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
    static ConstructorHelpers::FClassFinder<APawn> soldier(TEXT("/Game/ThirdPersonCPP/Blueprints/Soldier_BP"));
    static ConstructorHelpers::FClassFinder<APawn> healer(TEXT("/Game/ThirdPersonCPP/Blueprints/Healer_BP"));
    static ConstructorHelpers::FClassFinder<APawn> wizard(TEXT("/Game/ThirdPersonCPP/Blueprints/Wizard_BP"));
    static ConstructorHelpers::FClassFinder<APawn> assassin(TEXT("/Game/ThirdPersonCPP/Blueprints/Assassin_BP"));

    int32 randNumber = FMath::RandRange(0, 3);
    switch(randNumber)
    {
        case 1:
            if(healer.Class != NULL)
            {
                DefaultPawnClass = healer.Class;
                break;
            }
        case 2:
            if(wizard.Class != NULL)
            {
                DefaultPawnClass = wizard.Class;
                break;
            }
        case 3:
            if(assassin.Class != NULL)
            {
                DefaultPawnClass = assassin.Class;
                break;
            }
        case 0:
        default:
            if(soldier.Class != NULL)
            {
                DefaultPawnClass = soldier.Class;
                break;
            }
    }
    bUseSeamlessTravel = true;
}

void ARobotRebellionGameMode::BeginPlay()
{
    Super::BeginPlay();

    GameAlterationInstaller::getInstance().installAlteration<UStunAlteration>(&m_stunDefault);
    GameAlterationInstaller::getInstance().installAlteration<UInvisibilityAlteration>(&m_invisibleDefault);
}