// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "DamageCoefficientLogic.h"


bool DamageCoefficientLogic::establishCritical(const FName& boneName) const USE_NOEXCEPT
{
    static const TArray<FName> criticalBones{
        TEXT("Head"),
        TEXT("Neck_01"),
        TEXT("Spine3")
    };

    return criticalBones.Contains(boneName);
}