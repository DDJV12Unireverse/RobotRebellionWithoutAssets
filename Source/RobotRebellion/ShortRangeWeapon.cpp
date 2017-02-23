// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ShortRangeWeapon.h"
#include "RobotRebellionCharacter.h"


void UShortRangeWeapon::cppAttack(ARobotRebellionCharacter* user)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, "ShortAtt");
}

FString UShortRangeWeapon::rangeToFString() const USE_NOEXCEPT
{
    return "Short Range weapon";
}