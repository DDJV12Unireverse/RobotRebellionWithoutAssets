// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ShieldAlteration.h"

#include "Character/RobotRebellionCharacter.h"
#include "Gameplay/Attributes/Attributes.h"


UShieldAlteration::UShieldAlteration() :
    UAlterationBase()
{
    m_id = IdentifiableObject<UShieldAlteration>::ID;
}

void UShieldAlteration::destroyItself()
{
    m_alteredOwner->getAttributes()->removeShield(m_amount);
    m_alteredOwner->unspawnShieldParticle();

    this->DestroyComponent();
}

void UShieldAlteration::onCreate(ARobotRebellionCharacter* alteredOwner)
{
    m_alteredOwner = alteredOwner;
    m_alteredOwner->spawnShieldParticle();
    alteredOwner->getAttributes()->addShield(m_amount);
}


