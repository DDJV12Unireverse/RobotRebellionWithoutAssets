// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IWeaponBase.h"
#include "LongRangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ULongRangeWeapon : public UIWeaponBase
{
	GENERATED_BODY()
	

public:
    /************************************************************************/
    /*                  CONSTANT                                           */
    /************************************************************************/
    static constexpr const float LIFT_OFFSET = 10.f;




    /************************************************************************/
    /*                  UPROPERTY                                           */
    /************************************************************************/
    /** Projectile class */
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AProjectile> m_projectileClass;

    //Projectile position Offset
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        FVector m_muzzleOffset;




    /************************************************************************/
    /*                  UFUNCTION                                           */
    /************************************************************************/
    UFUNCTION(BlueprintCallable, Category = "General")
        EWeaponRange getWeaponRange() const USE_NOEXCEPT
    {
        return EWeaponRange::LONG_RANGE_WEAPON;
    }
	



    /************************************************************************/
    /*                  METHODS                                             */
    /************************************************************************/
    virtual void cppAttack(class ARobotRebellionCharacter* user) override;


    ULongRangeWeapon();
};
