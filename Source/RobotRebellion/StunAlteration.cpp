// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "StunAlteration.h"

#include "NonPlayableCharacter.h"
#include "PlayableCharacter.h"
#include "AIController.h"


void UStunAlteration::destroyItself()
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Black, "Destroy alteration");

    if (m_isNPC)
    {
        auto nonPlayableOwner = Cast<ANonPlayableCharacter>(m_alteredOwner);

        m_alteredActorController->SetPawn(nonPlayableOwner);

        Cast<AAIController>(m_alteredOwner->Controller)->GetBrainComponent()->RestartLogic();
    }
    else
    {
        auto playableOwner = Cast<APlayableCharacter>(m_alteredOwner);

        playableOwner->EnablePlayInput();
    }

    this->DestroyComponent();
}

void UStunAlteration::onCreate(ARobotRebellionCharacter* alteredOwner)
{
    m_alteredOwner = alteredOwner;

    m_alteredActorController = alteredOwner->Controller;

    auto playableOwner = Cast<APlayableCharacter>(alteredOwner);

    if (playableOwner)
    {
        m_isNPC = false;

        playableOwner->DisablePlayInput();
    }
    else
    {
        auto nonPlayableOwner = Cast<ANonPlayableCharacter>(alteredOwner);

        if (nonPlayableOwner)
        {
            m_isNPC = true;

            Cast<AAIController>(m_alteredOwner->Controller)->GetBrainComponent()->StopLogic("");
        }
        else
        {
            this->DestroyComponent();
        }
    }
}
