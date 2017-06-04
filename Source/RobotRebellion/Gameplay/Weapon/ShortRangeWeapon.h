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
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
        USoundCue* m_missOutsideSound;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
        USoundCue* m_missBigRoomSound;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
        USoundCue* m_missSmallRoomSound;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
        USoundCue* m_missCorridorSound;
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
    //    USoundCue* m_hitSound;


    // Weapon Fire Sound
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
        USoundCue* m_hitOutsideSound;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
        USoundCue* m_hitBigRoomSound;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
        USoundCue* m_hitSmallRoomSound;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
        USoundCue* m_hitCorridorSound;


    /************************************************************************/
    /*                  CONSTRUCTORS                                        */
    /************************************************************************/

    UShortRangeWeapon();


    /************************************************************************/
    /*                  UFUNCTION                                           */
    /************************************************************************/
    UFUNCTION(BlueprintCallable, Category = "General")
        virtual EWeaponRange getWeaponRange() const USE_NOEXCEPT override
    {
        return EWeaponRange::SHORT_RANGE_WEAPON;
    }

    UFUNCTION(NetMulticast, Reliable)
        virtual void playSound(USoundCue* sound, AActor* originator) override;


    /************************************************************************/
    /*                  METHODS                                             */
    /************************************************************************/

    virtual void cppAttack(class ARobotRebellionCharacter* user) override;


    virtual void cppAttack(ARobotRebellionCharacter* user, ARobotRebellionCharacter* ennemy) override;

    //virtual void playSound(ARobotRebellionCharacter* user) override;

    virtual FString rangeToFString() const USE_NOEXCEPT;

    virtual void inflictDamageLogic(ARobotRebellionCharacter* receiver, const FHitResult& hit);
};
