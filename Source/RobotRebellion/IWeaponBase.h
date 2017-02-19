// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PrimitiveComponent.h"
#include "UtilitaryMacros.h"
#include "Engine/EngineTypes.h"
#include "IWeaponBase.generated.h"


UENUM(BlueprintType)
enum class EWeaponRange : uint8
{
    SHORT_RANGE_WEAPON  UMETA(DisplayName = "Short Range Weapon"),
    LONG_RANGE_WEAPON   UMETA(DisplayName = "Long Range Weapon")
};

/**
 * 
 */
UCLASS(ABSTRACT, Blueprintable)
class ROBOTREBELLION_API UIWeaponBase : public UPrimitiveComponent
{
	GENERATED_BODY()


public:
    /************************************************************************/
    /*                  UPROPERTY                                           */
    /************************************************************************/
    
    //Damage multiplier. Specific to the weapon.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_weaponDamageCoefficient;

    //Weapon base damage. Specific to the weapon.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_weaponBaseDamage;

    //Weapon base damage. Specific to the weapon.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_weaponBaseCadence;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", ReplicatedUsing = waitForReloading)
        bool m_canAttack;



protected:
    /************************************************************************/
    /*                  PROPERTY                                            */
    /************************************************************************/
    FTimerHandle m_nextAllowedAttackTimer;




public:
    /************************************************************************/
    /*                  UFUNCTION                                           */
    /************************************************************************/

    UFUNCTION(BlueprintCallable, Category = "General")
#ifdef WE_RE_ON_DEBUG
        EWeaponRange getWeaponRange() const USE_NOEXCEPT PURE_VIRTUAL(UIWeaponBase::getWeaponRange, return EWeaponRange::SHORT_RANGE_WEAPON;)
#else //!WE_RE_ON_DEBUG
        EWeaponRange getWeaponRange() const USE_NOEXCEPT
    {
        return EWeaponRange::SHORT_RANGE_WEAPON;
    }
#endif //WE_RE_ON_DEBUG

    //DO NOT MOVE OR COMMENT !! It is the only way I found...
    void dummy1(){}

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
        bool canAttack() const USE_NOEXCEPT
    {
        return m_canAttack;
    }

    UFUNCTION()
    void waitForReloading();



    /************************************************************************/
    /*                  METHODS                                             */
    /************************************************************************/
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    virtual void cppAttack(ARobotRebellionCharacter* user)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, "BaseAtt");
    }

    void cannotAttack(float agility) 
    {}

    void reload();

    virtual FString toDebugFString() const USE_NOEXCEPT;



    UIWeaponBase();
};
