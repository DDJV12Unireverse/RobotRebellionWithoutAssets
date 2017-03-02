// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spell.h"
#include "ThrowSpell.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UThrowSpell : public USpell
{
	GENERATED_BODY()
public:
    // True if we want to impact the affected character
    // False if we just use the impact position
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw Settings")
        bool m_isTargetThrow;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw Settings")
        float m_liftOffset;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw Settings")
        float m_projectileInitialSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw Settings")
        FVector m_muzzleOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw Settings")
        TSubclassOf<class AProjectileEffect> m_projectileClass;
    
    virtual void cast() override;

    void onHit(class UPrimitiveComponent*, class AActor*, class UPrimitiveComponent*, FVector, const FHitResult&);

    void applicateEffect(class ARobotRebellionCharacter* affectedTarget);
    void applicateEffect(FVector impactPoint);
	
};
