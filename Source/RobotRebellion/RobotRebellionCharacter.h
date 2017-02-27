// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Attributes.h"
#include "GameFramework/Character.h"

#include "RobotRebellionCharacter.generated.h"


/*
 * Mother class for every character in RobotRebellion Game
 */
UCLASS(config = Game)
class ARobotRebellionCharacter : public ACharacter
{
    GENERATED_BODY()


public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
        float m_moveSpeed;

     ////Weapon Inventory/////
     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
         class UWeaponInventory* m_weaponInventory;


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute, meta = (AllowPrivateAccess = "true"), Replicated)
        UAttributes* m_attribute;


public:

    ARobotRebellionCharacter();
    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/
    class UWeaponBase* getCurrentEquippedWeapon() const USE_NOEXCEPT;

     virtual void BeginPlay() override;

    ////Server
     void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    virtual void cppOnRevive();

    virtual void cppOnDeath();


     UFUNCTION()
         void onDeath();

     UFUNCTION(Reliable, Client, WithValidation)
         void clientOnDeath();



// Attributs relatives functions added by macro
public:
    GENERATED_USING_AND_METHODS_FROM_Attributes(m_attribute, ->);
};

