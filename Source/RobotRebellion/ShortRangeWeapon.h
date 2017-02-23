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

    /************************************************************************/
    /*                  UPROPERTY                                           */
    /************************************************************************/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range")
        float m_weaponForwardRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range")
        float m_weaponVerticallyRange;

    /************************************************************************/
    /*                  CONSTRUCTORS                                        */
    /************************************************************************/

    UShortRangeWeapon();
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
    virtual void cppAttack(class ARobotRebellionCharacter* user) override;
};
