// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponBase.h"
#include "LongRangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ULongRangeWeapon : public UWeaponBase
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
        virtual EWeaponRange getWeaponRange() const USE_NOEXCEPT override
    {
        return EWeaponRange::LONG_RANGE_WEAPON;
    }

    /************************************************************************/
    /*                  METHODS                                             */
    /************************************************************************/
    virtual void cppAttack(class ARobotRebellionCharacter* user) override;

    virtual FString rangeToFString() const USE_NOEXCEPT;
    


    ULongRangeWeapon();
};
