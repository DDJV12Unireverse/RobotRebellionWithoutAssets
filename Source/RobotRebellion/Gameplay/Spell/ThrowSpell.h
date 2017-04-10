// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spell.h"
#include "ThrowSpell.generated.h"

/**
 * Implement logic for spell which invoke a projectile with an initiale speed
 *      The projectil will trigger an onhit effect. 
 *      this effect could be targeted or just use an impact
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

    /** offset on the pitch rotation to emulate nade throw */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw Settings")
        float m_liftOffset;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw Settings")
        FVector m_muzzleOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw Settings")
        TSubclassOf<class AProjectileEffect> m_projectileClass;
    

public:
    UThrowSpell();
    virtual void BeginPlay() override;

    virtual void cast() override;

    // Call by the projectile once it hit smth
    void onHit(class UPrimitiveComponent*, class AActor*, class UPrimitiveComponent*, FVector, const FHitResult&);

    // Apply Effects on a target that have to be a RobotRebellion Character
    void applyEffect(class ARobotRebellionCharacter* affectedTarget);
    // Aplly Effects on a specific location
    void applyEffect(FVector impactPoint);
	
};
