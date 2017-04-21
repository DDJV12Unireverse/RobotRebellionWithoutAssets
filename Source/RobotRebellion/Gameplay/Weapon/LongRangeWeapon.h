// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponBase.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
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

    // Weapon Fire Sound
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
        USoundCue* m_longRangeWeaponFireSound;

    /************************************************************************/
    /*                  PROPERTY                                            */
    /************************************************************************/


    /************************************************************************/
    /*                  UFUNCTION                                           */
    /************************************************************************/
    UFUNCTION(BlueprintCallable, Category = "General")
        virtual EWeaponRange getWeaponRange() const USE_NOEXCEPT override
    {
        return EWeaponRange::LONG_RANGE_WEAPON;
    }

    UFUNCTION(NetMulticast, Reliable)
        virtual void playSound(USoundCue* sound, AActor* originator) override;

    /************************************************************************/
    /*                  METHODS                                             */
    /************************************************************************/

    ULongRangeWeapon();

    virtual void cppAttack(class ARobotRebellionCharacter* user) override;
    
    virtual void cppAttack(ARobotRebellionCharacter* instigator, ARobotRebellionCharacter* ennemy) override;

    virtual FString rangeToFString() const USE_NOEXCEPT;

    void fireMethod(class AProjectile* projectile, const FVector& fireDirection);
};
