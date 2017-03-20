// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Gameplay/Attributes/Attributes.h"
#include "../Gameplay/Alteration/AlterationController.h"
#include "../UI/ELivingTextAnimMode.h"
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
        TSubclassOf<class UTextBillboardComponent> m_textBillboardDefault;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BillBoard")
        class UTextBillboardComponent* m_textBillboardInstance;


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute, meta = (AllowPrivateAccess = "true"), Replicated)
        UAttributes* m_attribute;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute, meta = (AllowPrivateAccess = "true"))
        UAlterationController* m_alterationController;



    /************************************************************************/
    /* PROPERTY                                                             */
    /************************************************************************/
    void(ARobotRebellionCharacter::* m_timedDestroyDelegate)(float deltaTime);
    void(ARobotRebellionCharacter::* m_disableBeforeDestroyDelegate)();




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


    void startTimedDestroy() USE_NOEXCEPT;

    void inflictStun();
    void inflictStun(float duration);

    void inflictInvisibility();

    void doesNothing() {}


protected:
    FORCEINLINE void noDestroyForNow(float deltaTime)
    {}

    void destroyNow(float deltaTime);

    FORCEINLINE void endDisabling()
    {}

    void disablingEverything();


public:
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
        void createTextBillboardWithThisCamera(UCameraComponent* camera);

    UFUNCTION(BlueprintCallable, Category = "Billboard - Living Text")
        void displayAnimatedIntegerValue(int32 valueToDisplay, const FColor& color, ELivingTextAnimMode mode = ELivingTextAnimMode::TEXT_ANIM_MOVING);

    UFUNCTION(BlueprintCallable, Category = "Billboard - Living Text")
        void displayAnimatedText(const FString& textToDisplay, const FColor& color, ELivingTextAnimMode mode = ELivingTextAnimMode::TEXT_ANIM_MOVING);


    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void netMultidisplayAnimatedIntegerValue(int32 valueToDisplay, const FColor& color, ELivingTextAnimMode mode);

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void netMultidisplayAnimatedText(const FString& textToDisplay, const FColor& color, ELivingTextAnimMode mode);


    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void netMultiKill();

    UFUNCTION()
        void setInvisible(bool isInvisible);

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiSetInvisible(bool isInvisible);


// Attributs relatives functions added by macro
public:
    GENERATED_USING_AND_METHODS_FROM_Attributes(m_attribute, ->);
};

