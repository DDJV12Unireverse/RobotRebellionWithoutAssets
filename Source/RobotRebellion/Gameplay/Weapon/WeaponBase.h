// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PrimitiveComponent.h"
#include "../../Tool/UtilitaryMacros.h"
#include "Engine/EngineTypes.h"
#include "WeaponBase.generated.h"


UENUM(BlueprintType)
enum class EWeaponRange : uint8
{
    SHORT_RANGE_WEAPON   UMETA(DisplayName = "Short Range Weapon"),
    LONG_RANGE_WEAPON    UMETA(DisplayName = "Long Range Weapon"),
    INVALID_RANGE_WEAPON UMETA(DisplayName = "Invalid Range Weapon"),
};

/**
 *
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API UWeaponBase : public UPrimitiveComponent
{
    GENERATED_BODY()


public:
    /************************************************************************/
    /*                  UPROPERTY                                           */
    /************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Owner")
        AActor* m_owner;

    //Damage multiplier. Specific to the weapon.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_weaponDamageCoefficient;

    //Weapon base damage. Specific to the weapon.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_weaponBaseDamage;

    //Weapon base damage. Specific to the weapon.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_weaponBaseCadence;

    //Projectile position Offset
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        float m_WeaponRadiusRange;

protected:
    /************************************************************************/
    /*                  PROPERTY                                            */
    /************************************************************************/
    double m_nextAllowedAttackTimer;




public:
    /************************************************************************/
    /*                  UFUNCTION                                           */
    /************************************************************************/

    UFUNCTION(BlueprintCallable, Category = "General")
        virtual EWeaponRange getWeaponRange() const USE_NOEXCEPT
    {
        //check(false); // To disable instanciation of base class.
        return EWeaponRange::INVALID_RANGE_WEAPON;
    }


    UFUNCTION(BlueprintCallable, Category = "General")
        FString toFString() const USE_NOEXCEPT
    {
        return this->toDebugFString();
    }

    UFUNCTION(BlueprintCallable, Category = "Action")
        void attack(class ARobotRebellionCharacter* user)
    {
        return this->cppAttack(user);
    }

    UFUNCTION(BlueprintCallable, Category = "Action")
        bool canAttack() const USE_NOEXCEPT;

    UFUNCTION(BlueprintCallable, Category = "Action")
        void reload();



    /************************************************************************/
    /*                  METHODS                                             */
    /************************************************************************/
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    virtual void cppAttack(ARobotRebellionCharacter* user)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, "BaseAtt");
    }

    virtual void cppAttack(ARobotRebellionCharacter* user, ARobotRebellionCharacter* ennemy)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, "BaseAtt instigator to ennemy");
    }

    virtual void playSound(USoundCue* sound, AActor* originator) PURE_VIRTUAL(UWeaponBase::playSound, )

    virtual FString rangeToFString() const USE_NOEXCEPT;

    virtual FString toDebugFString() const USE_NOEXCEPT;


    UWeaponBase();
};
