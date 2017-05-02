// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RobotRebellionCharacter.h"
#include "ClassType.h"
#include "../Gameplay/Spell/SpellKit.h"
#include "../Gameplay/Item/Focusable.h"
#include "Location.h"
#include "PlayableCharacter.generated.h"

/**
 *  Playable Character for Robot Rebellion Game
 */
UCLASS()
class ROBOTREBELLION_API APlayableCharacter : public ARobotRebellionCharacter, public Focusable
{
    GENERATED_BODY()

public:
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
        class USkeletalMeshComponent* m_fpsMesh;

public:
    ////Sprint////
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_SprintButtonDown)
        bool m_bPressedRun;

    ////CROUCH////
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_CrouchButtonDown)
        bool m_bPressedCrouch;
    
    ////INVENTORY
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", Replicated)
        int m_healthPotionsCount;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", Replicated)
        int m_manaPotionsCount;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", Replicated)
        int m_bombCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
        int m_nbHealthPotionStart;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
        int m_nbManaPotionStart;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
        int m_nbBombStart;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
        int m_nbHealthPotionMax;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
        int m_nbManaPotionMax;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
        int m_nbBombMax;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
        float m_healthPerPotion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
        float m_manaPerPotion;

    //Reviving Count
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reviving")
        float m_requiredTimeToRevive;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reviving")
        float m_currentRevivingTime;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reviving")
        bool m_isReviving;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseLookUpRate;

    //camera broom distance from player pawn while in tps mode
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
        float m_TPSCameraDistance;

    //camera broom distance from player pawn while in fps mode
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
        float m_FPSCameraDistance;

    //camera broom distance from player pawn while in tps mode
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
        FVector m_mireOffset;

    // Maximal Focus distance on items.
    UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
        float MinUseDistance;
    UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
        float MaxUseDistance;

    // Seulement vrai lors de la premi�re image avec un nouveau focus.
    bool bHasNewFocus;
    AActor* focusedPickupActor;

    bool m_tpsMode;

    bool m_isBurnEffectEnabled;

    ELocation m_location;

    void(APlayableCharacter::* deactivatePhysicsKilledMethodPtr)();


public:
    APlayableCharacter();


private:
    void doesNothing(){}
    void deactivatePhysicsWhenKilled();

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

    virtual void BeginPlay() override;

    ////Server
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    void updateIfInCombat();

    UFUNCTION()
        void cppPreRevive(APlayableCharacter* characterToRevive);

    UFUNCTION(Reliable, Server, WithValidation)
        virtual void serverCppPreRevive(APlayableCharacter* characterToRevive);

    UFUNCTION(BlueprintCallable, Category = "Revive")
        void revive()
    {
        this->cppOnRevive();
    }

    virtual void cppOnRevive() override;
    virtual void cppOnDeath() override;

    void EnablePlayInput(bool enable);

    void inputOnLiving(class UInputComponent* playerInput);
    void inputOnDying(class UInputComponent* playerInput);

    void inputDebug(class UInputComponent* playerInput);

    ////Command Line
    UFUNCTION(BlueprintCallable, Category = "CharacterCommand")
        void ExecuteCommand(FString command) const;

    //////UI
    void openLobbyWidget();

    UFUNCTION(BlueprintCallable, Category = LobbyWidget)
    void closeLobbyWidget();

    UFUNCTION(BlueprintCallable, Category = CharacterSelection)
        void closeSelectionWidget();

    void giveInputGameMode(bool status);

    /************************************************************************/
    /* UFUNCTION                                                            */
    /************************************************************************/
    UFUNCTION()
        void OnStartJump();

    // On d�sactive le bool�en bPressedJump
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
        if (Role < ROLE_Authority)
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

    UFUNCTION(Reliable, Server, WithValidation)
        void serverSwitchWeapon();

    UFUNCTION()
        void interactBegin();

    UFUNCTION()
        void interact(AActor* focusedActor);

    UFUNCTION()
        void interactEnd();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverInteract(AActor* focusedActor);

    UFUNCTION(NetMulticast, Reliable)
        void clientInteract(APickupActor* Usable);


    UFUNCTION(NetMulticast, Reliable)
        void clientRevive();


    UFUNCTION(Reliable, Client, WithValidation)
        void clientEnableInput(bool enableInput);

    // Called every image
    virtual void Tick(float DeltaSeconds) override;

    AActor* GetUsableInView();

    //////INVENTORY///////

    void useHealthPotion();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverUseHealthPotion();

    void useManaPotion();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverUseManaPotion();

    void loseBomb();
    UFUNCTION(Reliable, Server, WithValidation)
        void serverLoseBomb();

    //Activate the collision physic if true, deactivate otherwise
    UFUNCTION(BlueprintCallable, Category = "Physics")
        void activatePhysics(bool mustActive);

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiActivatePhysics(bool mustActive);


    UFUNCTION()
        void onDebugCheat();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverOnDebugCheat();

    UFUNCTION()
        void gotoDesert();
    UFUNCTION()
        void gotoRuins();
    UFUNCTION()
        void gotoGym();
    UFUNCTION(Reliable, Server, WithValidation)
        void serverGotoDesert();
    UFUNCTION(Reliable, Server, WithValidation)
        void serverGotoRuins();
    UFUNCTION(Reliable, Server, WithValidation)
        void serverGotoGym();

    UFUNCTION(BlueprintCallable, Category = "ReviveTimer")
        float getReviveTimer() const USE_NOEXCEPT
    {
        return m_currentRevivingTime;
    }
    UFUNCTION(BlueprintCallable, Category = "ReviveTimer")
        float getRequiredReviveTime() const USE_NOEXCEPT
    {
        return m_requiredTimeToRevive;
    }
    UFUNCTION(BlueprintCallable, Category = "ReviveTimer")
        bool isReviving() const USE_NOEXCEPT
    {
        return m_isReviving;
    }

    UFUNCTION()
    void giveBombToDrone(ADroneAIController* drone);

    UFUNCTION(Reliable, Server, WithValidation)
        void serverGiveBombToDrone(ADroneAIController* drone);

    

    int getManaPotionCount()
    {
        return m_manaPotionsCount;
    }

    int getHealthPotionCount()
    {
        return m_healthPotionsCount;
    }

    int getBombCount()
    {
        return m_bombCount;
    }

    void setManaPotionCount(int nbPotion);

    void setHealthPotionCount(int nbPotion);

    void setBombCount(int nbBombs);

    void switchView();

    UMeshComponent* getCurrentViewMesh();

    virtual void OnPickup(APawn* InstigatorPawn) override;

    virtual void OnBeginFocus() override
    {}
    virtual void OnEndFocus() override
    {}

    void enableDroneDisplay();

    UFUNCTION(Reliable, Client)
        void updateAllCharacterBillboard(UCameraComponent* camToFollow);


    void disableFireEffect();

    ELocation GetLocation()
    {
        return m_location;
    }
    void setLocation(ELocation location)
    {
        m_location=location;
    }
};
