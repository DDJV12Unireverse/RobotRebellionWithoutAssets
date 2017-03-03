// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RobotRebellionCharacter.h"
#include "ClassType.h"
#include "SpellKit.h" 
#include "PlayableCharacter.generated.h"

/**
 *  Playable Character for Robot Rebellion Game
 */
UCLASS()
class ROBOTREBELLION_API APlayableCharacter : public ARobotRebellionCharacter
{
	GENERATED_BODY()
	
private:
    // COMPONENT
    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;
    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* FollowCamera;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
        class URobotRobellionSpawnerClass* m_spawner;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellKit", meta = (AllowPrivateAccess = "true"))
        USpellKit* m_spellKit;

public:
    ////Sprint////
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_SprintButtonDown)
        bool m_bPressedRun;

    ////CROUCH////
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_CrouchButtonDown)
        bool m_bPressedCrouch;

//     ////Weapon Inventory/////
//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
//         class UWeaponInventory* m_weaponInventory;

        /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseLookUpRate;

    // Distance maximale de focus sur les objets.
    UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
        float MaxUseDistance;

    // Seulement vrai lors de la première image avec un nouveau focus.
    bool bHasNewFocus;
    class APickupActor* focusedPickupActor;

public:
    APlayableCharacter();

protected:
    /************************************************************************/
    /* METHODES                                                            */
    /************************************************************************/

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

    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // End of APawn interface
    virtual EClassType getClassType() const USE_NOEXCEPT;


    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const
    {
        return CameraBoom;
    }

    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const
    {
        return FollowCamera;
    }

//    class UWeaponBase* getCurrentEquippedWeapon() const USE_NOEXCEPT;

    virtual void BeginPlay() override;

    ////Server
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    virtual void cppOnRevive() override;
    virtual void cppOnDeath() override;

    void inputOnLiving(class UInputComponent* playerInput);
    void inputOnDying(class UInputComponent* playerInput);

    void inputDebug(class UInputComponent* playerInput);

    ////Command Line
    UFUNCTION(BlueprintCallable, Category = "CharacterCommand")
        void ExecuteCommand(FString command) const;

    //////UI
    void openLobbyWidget();

    /************************************************************************/
    /* UFUNCTION                                                            */
    /************************************************************************/
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


    ///// WORLD INFO
    UFUNCTION(BlueprintCallable, Category = "World Info")
        FString GetCurrentMapName()
    {
        return GetWorld()->GetMapName();
    }

    /////FIRE
    UFUNCTION()
        void mainFire();


    UFUNCTION(Reliable, Server, WithValidation)
        void serverMainFire();

    //CAST SPELL
    template<int32 index>
    void castSpellInputHanlder()
    {
        if(Role < ROLE_Authority)
        {
            castSpellServer(index); // le param n'a pas d'importance pour l'instant
        }
        else
        {
            castSpell(index);
        }
    }

    UFUNCTION()
        void castSpell(int32 index);

    UFUNCTION(Reliable, Server, WithValidation)
        void castSpellServer(int32 index);

    //DEATH
    
    //Function to call in BP, can't do it with macro
    UFUNCTION(BlueprintCallable, Category = "General")
        bool isDeadBP();
    


    //Type
    UFUNCTION(BlueprintCallable, Category = "General")
        EClassType getType() const USE_NOEXCEPT;


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

    UFUNCTION()
        void interact();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverInteract();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverSwitchWeapon();

    // Called every image
    virtual void Tick(float DeltaSeconds) override;

    class APickupActor* GetUsableInView();
};
