// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ReviveTimerWidget.h"
#include "../Character/PlayableCharacter.h"

void UReviveTimerWidget::getTimerRatio(float& ratio, float& currentTime, float& requiredTime) const
{
    APlayableCharacter* character = Cast<APlayableCharacter>(GetOwningPlayerPawn());

    if (character)
    {
        currentTime = character->getReviveTimer();
        requiredTime = character->getRequiredReviveTime();
        ratio = currentTime / requiredTime;
    }
    else
    {
        currentTime = 0.f;
        requiredTime = 0.f;
        ratio = 0.f;
    }
}



