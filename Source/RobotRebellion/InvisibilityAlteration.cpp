// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "InvisibilityAlteration.h"


#include "NonPlayableCharacter.h"
#include "PlayableCharacter.h"


UAlterationBase::ID UInvisibilityAlteration::m_id;


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
