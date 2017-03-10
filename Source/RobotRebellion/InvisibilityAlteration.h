// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlterationBase.h"
#include "IdentifiableObj.h"
#include "InvisibilityAlteration.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API UInvisibilityAlteration : public UAlterationBase
{
	GENERATED_BODY()
	
	
public:
    UInvisibilityAlteration();


    void destroyItself() override;

    void onCreate(class ARobotRebellionCharacter* alteredOwner) override;

    virtual FString toDebugString() const USE_NOEXCEPT
    {
        return "Invisibility";
    }
};
