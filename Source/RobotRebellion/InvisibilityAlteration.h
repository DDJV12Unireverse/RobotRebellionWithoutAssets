// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlterationBase.h"
#include "InvisibilityAlteration.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UInvisibilityAlteration : public UAlterationBase
{
	GENERATED_BODY()
	
	
public:
    static UAlterationBase::ID m_id;


public:
    void destroyItself() override;

    void onCreate(class ARobotRebellionCharacter* alteredOwner) override;

    virtual FString toDebugString() const USE_NOEXCEPT
    {
        return "Invisibility";
    }
};
