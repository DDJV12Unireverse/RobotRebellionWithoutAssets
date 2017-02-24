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
	
private:

    /************************************************************************/
    /*                  UPROPERTY                                           */
    /************************************************************************/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range", meta = (AllowPrivateAccess = "true"))
        float m_weaponForwardRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range", meta = (AllowPrivateAccess = "true"))
        float m_weaponVerticallyRange;

public:
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
