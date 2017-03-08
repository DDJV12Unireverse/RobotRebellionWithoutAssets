// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlterationBase.h"
#include "StunAlteration.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API UStunAlteration : public UAlterationBase
{
	GENERATED_BODY()
	
public:
    static UAlterationBase::ID m_id;


    class AController* m_alteredActorController;


public:
    virtual void BeginPlay() override;

    void destroyItself() override;

    void onCreate(class ARobotRebellionCharacter* alteredOwner) override;

    virtual FString toDebugString() const USE_NOEXCEPT
    {
        return "Stun";
    }
};
