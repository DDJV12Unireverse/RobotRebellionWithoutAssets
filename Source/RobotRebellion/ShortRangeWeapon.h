// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponBase.h"
#include "ShortRangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UShortRangeWeapon : public UWeaponBase
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Collision")
        UCapsuleComponent* m_attackZone;
	
public:
    /************************************************************************/
    /*                  CONSTRUCTOR                                           */
    /************************************************************************/
    UShortRangeWeapon();

    /************************************************************************/
    /*                  UFUNCTION                                           */
    /************************************************************************/
    UFUNCTION()
        void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
    
    UFUNCTION(BlueprintCallable, Category = "General")
        EWeaponRange getWeaponRange() const USE_NOEXCEPT
    {
        return EWeaponRange::SHORT_RANGE_WEAPON;
    }



    /************************************************************************/
    /*                  METHODS                                             */
    /************************************************************************/
    virtual void cppAttack(class ARobotRebellionCharacter* user) override;
};
