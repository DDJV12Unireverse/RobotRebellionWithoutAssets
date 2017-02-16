// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Attributes.h"
#include "GameFramework/Character.h"
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
    ARobotRebellionCharacter();

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseLookUpRate;

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

protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // End of APawn interface

public:

    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    /////////////////////////////////////////////////////////ADDED ATTRIBUTES AND FUNCTIONS:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
        float m_moveSpeed;


    //On active le booléen bPressedJump
    UFUNCTION()
        void OnStartJump();
    // On désactive le booléen bPressedJump
    UFUNCTION()
        void OnStopJump();

    ////Sprint////

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_SprintButtonDown)
        bool m_bPressedRun;

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

    ////CROUCH////
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_CrouchButtonDown)
        bool m_bPressedCrouch;

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

    /** Projectile class */
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AProjectile> ProjectileClass;
    //Projectile position Offset
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        FVector MuzzleOffset;

    UFUNCTION()
        void mainFire();


    UFUNCTION(Reliable, Server, WithValidation)
        void serverMainFire();

};

