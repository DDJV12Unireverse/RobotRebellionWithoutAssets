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

    ////Billboard on character////
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BillBoard")
        class UTextBillboardComponent* m_textBillboardDefault;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BillBoard")
        class UTextBillboardComponent* m_textBillboardInstance;


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute, meta = (AllowPrivateAccess = "true"), Replicated)
        UAttributes* m_attribute;


public:
    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/


    ARobotRebellionCharacter();


    class UWeaponBase* getCurrentEquippedWeapon() const USE_NOEXCEPT;

    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;


   ////Server
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


    virtual void cppOnRevive();

    virtual void cppOnDeath();



    /************************************************************************/
    /* UFUNCTION                                                            */
    /************************************************************************/

    UFUNCTION()
        void onDeath();

    UFUNCTION(Reliable, Client, WithValidation)
        void clientOnDeath();

    UFUNCTION(BlueprintCallable, Category = "Billboard - Living Text")
        void createTextBillboard();

    UFUNCTION(BlueprintCallable, Category = "Billboard - Living Text")
        void displayAnimatedIntegerValue(int32 valueToDisplay, const FColor& color);

    UFUNCTION(BlueprintCallable, Category = "Billboard - Living Text")
        void displayAnimatedTextValue(const FString& textToDisplay, const FColor& color);



// Attributs relatives functions added by macro
public:
    GENERATED_USING_AND_METHODS_FROM_Attributes(m_attribute, ->);
};

