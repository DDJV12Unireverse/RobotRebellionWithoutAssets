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


    bool m_isNPC;
    class AController* m_alteredActorController;


public:
    void destroyItself() override;

    void onCreate(class ARobotRebellionCharacter* alteredOwner) override;

    virtual FString toDebugString() const USE_NOEXCEPT
    {
        return "Stun";
    }

    //No, it isn't the way Patrice taught us but I don't have the time luxury to implement it the correct way.
    virtual ID getID() const USE_NOEXCEPT override
    {
        return UStunAlteration::m_id;
    }
};
