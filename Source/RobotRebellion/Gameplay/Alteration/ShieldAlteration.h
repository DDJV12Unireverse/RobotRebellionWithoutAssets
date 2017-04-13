// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Alteration/AlterationBase.h"
#include "ShieldAlteration.generated.h"

/**
 * Class about the shield alteration
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API UShieldAlteration : public UAlterationBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_amount;

    UShieldAlteration();

    void destroyItself() override;

    void onCreate(class ARobotRebellionCharacter* alteredOwner) override;

    virtual FString toDebugString() const USE_NOEXCEPT
    {
        return "Shield";
    }
};
