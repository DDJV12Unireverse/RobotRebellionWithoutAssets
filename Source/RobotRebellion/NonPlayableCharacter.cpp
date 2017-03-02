// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "NonPlayableCharacter.h"

#include "TextBillboardComponent.h"

ANonPlayableCharacter::ANonPlayableCharacter() : ARobotRebellionCharacter()
{
    // fill it

}


void ANonPlayableCharacter::cppOnDeath()
{
    if (this->m_textBillboardInstance)
    {
        this->m_textBillboardInstance->clearAllLivingTexts();
    }

    this->Destroy();
}