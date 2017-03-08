// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "InvisibilityAlteration.h"


#include "NonPlayableCharacter.h"
#include "PlayableCharacter.h"


UAlterationBase::ID UInvisibilityAlteration::m_id;


void UInvisibilityAlteration::destroyItself()
{
    UMeshComponent* characterMesh = m_alteredOwner->FindComponentByClass<UMeshComponent>();
    if (characterMesh)
    {
        characterMesh->SetVisibility(true);
    }

    this->DestroyComponent();
}

void UInvisibilityAlteration::onCreate(ARobotRebellionCharacter* alteredOwner)
{
    m_alteredOwner = alteredOwner;

    UMeshComponent* characterMesh = m_alteredOwner->FindComponentByClass<UMeshComponent>();
    if (characterMesh)
    {
        characterMesh->SetVisibility(false);
    }
    else
    {
        this->DestroyComponent();
    }
}

