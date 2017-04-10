// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "InvisibilityAlteration.h"


#include "Character/NonPlayableCharacter.h"
#include "Character/PlayableCharacter.h"


UInvisibilityAlteration::UInvisibilityAlteration() :
    UAlterationBase()
{
    m_id = IdentifiableObject<UInvisibilityAlteration>::ID;
}

void UInvisibilityAlteration::destroyItself()
{
    m_alteredOwner->setInvisible(false);

    this->DestroyComponent();
}

void UInvisibilityAlteration::onCreate(ARobotRebellionCharacter* alteredOwner)
{
    m_alteredOwner = alteredOwner;

    alteredOwner->setInvisible(true);
}
