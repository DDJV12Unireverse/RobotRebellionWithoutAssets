// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IWeaponBase.h"
#include "ShortRangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UShortRangeWeapon : public UIWeaponBase
{
	GENERATED_BODY()
	
	
public:
    /************************************************************************/
    /*                  UFUNCTION                                           */
    /************************************************************************/
    UFUNCTION(BlueprintCallable, Category = "General")
        EWeaponRange getWeaponRange() const USE_NOEXCEPT
    {
        return EWeaponRange::SHORT_RANGE_WEAPON;
    }



    /************************************************************************/
    /*                  METHODS                                             */
    /************************************************************************/
    virtual void cppAttack(float agility) override;
};
