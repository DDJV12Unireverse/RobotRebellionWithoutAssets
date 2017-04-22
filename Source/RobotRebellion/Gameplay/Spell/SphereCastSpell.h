// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Spell/Spell.h"
#include "SphereCastSpell.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API USphereCastSpell : public USpell
{
	GENERATED_BODY()
	
public:
    /** the distance betweent the start location and the end location of the spherecast */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SphereCastSpell)
        float m_length;

    /** radius of the casted shpere*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SphereCastSpell)
        float m_sphereRadius;

    /** acctor spawnable for effect visual*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VisualEffect)
        TSubclassOf<AActor> m_effectActor;
    /** specifie if the effect must be moved away from the caster*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VisualEffect)
        float m_offset;
    /** specifie if the effect must be moved away from the caster*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VisualEffect)
        bool m_useEffect;

public:
    USphereCastSpell();

    virtual void BeginPlay() override;

    virtual void cast() override;

    // Apply Effects on a target that have to be a RobotRebellion Character
    void applyEffect(class ARobotRebellionCharacter* affectedTarget);
    // Aplly Effects on a specific location
    void applyEffect(FVector impactPoint);
	
private:
    FVector getRealAimingVector(const ARobotRebellionCharacter* caster);
};
