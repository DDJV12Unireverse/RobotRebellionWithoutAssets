// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Attributes.h"
#include "GameFramework/Character.h"
#include "ClassType.h"
#include "RobotRebellionCharacter.generated.h"



UCLASS(config = Game)
class ARobotRebellionCharacter : public ACharacter//, public Attributes
{
    GENERATED_BODY()


    /** Camera boom positioning the camera behind the character */
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* FollowCamera;


public:
    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseLookUpRate;


public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute, meta = (AllowPrivateAccess = "true"), Replicated)
        UAttributes* m_attribute;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
        class URobotRobellionSpawnerClass* m_spawner;


    /////////////////////////////////////////////////////////ADDED ATTRIBUTES AND FUNCTIONS:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
        float m_moveSpeed;

    ////Sprint////
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_SprintButtonDown)
        bool m_bPressedRun;

    ////CROUCH////
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_CrouchButtonDown)
        bool m_bPressedCrouch;

    ////Weapon Inventory/////
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
        class UWeaponInventory* m_weaponInventory;


public:
    ARobotRebellionCharacter();


protected:

    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    /**
     * Called via input to turn at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);



public:
    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

    //virtual void Tick(float DeltaTime) override;

    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // End of APawn interface

    virtual EClassType getClassType() const USE_NOEXCEPT;


    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    class UWeaponBase* getCurrentEquippedWeapon() const USE_NOEXCEPT;

    virtual void BeginPlay() override;

    /************************************************************************/
    /* UFUNCTION                                                            */
    /************************************************************************/

    //On active le booléen bPressedJump
    UFUNCTION()
        void OnStartJump();

    // On désactive le booléen bPressedJump
    UFUNCTION()
        void OnStopJump();

    UFUNCTION()
        void OnStartSprint();

    UFUNCTION()
        void OnStopSprint();

    UFUNCTION(BlueprintCallable, Category = "Movement")
        bool IsRunning()
    {
        return m_bPressedRun;
    }

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerSprintActivate(bool NewRunning);

    UFUNCTION()
        void OnRep_SprintButtonDown();

    void OnCrouchToggle();

    UFUNCTION(BlueprintCallable, Category = "Movement")
        bool IsCrouched()
    {
        return m_bPressedCrouch;
    }

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerCrouchToggle(bool NewCrouching);


    UFUNCTION()
        void OnRep_CrouchButtonDown();


    ////Server
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


    /////FIRE
    UFUNCTION()
        void mainFire();


    UFUNCTION(Reliable, Server, WithValidation)
        void serverMainFire();

    UFUNCTION(BlueprintCallable, Category = "General")
        EClassType getType() const USE_NOEXCEPT;



    //////////////////////////////////////////////////////////////////////////
    // Debug / cheats

    //Parse the class type to a string
    UFUNCTION(BlueprintCallable, Category = "Debug")
        FString typeToString() const USE_NOEXCEPT;

    //change instance to the specified instance type. Can be executed from command line
    UFUNCTION(BlueprintCallable, Category = "Debug", Exec)
        void changeInstanceTo(EClassType toType);

    UFUNCTION(BlueprintCallable, Category = "Debug")
        void changeToAssassin();

    UFUNCTION(BlueprintCallable, Category = "Debug")
        void changeToHealer();

    UFUNCTION(BlueprintCallable, Category = "Debug")
        void changeToSoldier();

    UFUNCTION(BlueprintCallable, Category = "Debug")
        void changeToWizard();

    UFUNCTION()
        void switchWeapon();


    UFUNCTION(Reliable, Server, WithValidation)
        void serverSwitchWeapon();


public:
    GENERATED_USING_AND_METHODS_FROM_Attributes(m_attribute, ->);
};

