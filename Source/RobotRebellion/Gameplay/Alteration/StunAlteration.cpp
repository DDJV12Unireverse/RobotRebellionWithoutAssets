// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "StunAlteration.h"

#include "Character/NonPlayableCharacter.h"
#include "Character/PlayableCharacter.h"
#include "AIController.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"



UStunAlteration::UStunAlteration() :
    UAlterationBase()
{
    m_id = IdentifiableObject<UStunAlteration>::ID;
}

void UStunAlteration::destroyItself()
{
    if(m_isNPC)
    {
        auto nonPlayableOwner = Cast<ANonPlayableCharacter>(m_alteredOwner);

        if(m_alteredActorController)
        {
            m_alteredActorController->SetPawn(nonPlayableOwner);

            AAIController* controller = Cast<AAIController>(m_alteredActorController);
            if(controller)
            {
                auto brain = controller->GetBrainComponent();

                if(brain)
                {
                    brain->RestartLogic();
                }
            }
        }
    }
    else
    {
        APlayableCharacter* playableOwner = Cast<APlayableCharacter>(m_alteredOwner);
        if(playableOwner)
        {
            playableOwner->EnablePlayInput(true);
        }
    }

    this->DestroyComponent();
}

void UStunAlteration::onCreate(ARobotRebellionCharacter* alteredOwner)
{
    m_alteredOwner = alteredOwner;

    m_alteredActorController = alteredOwner->Controller;

    auto playableOwner = Cast<APlayableCharacter>(alteredOwner);

    if(playableOwner)
    {
        m_isNPC = false;

        playableOwner->EnablePlayInput(false);
    }
    else
    {
        auto nonPlayableOwner = Cast<ANonPlayableCharacter>(alteredOwner);

        if(nonPlayableOwner)
        {
            m_isNPC = true;

            AAIController* controller = Cast<AAIController>(m_alteredOwner->Controller);
            if(controller)
            {
                auto brain = controller->GetBrainComponent();

                if(brain)
                {
                    brain->StopLogic("");
                }
            }
        }
        else
        {
            this->DestroyComponent();
        }
    }
}
