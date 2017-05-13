// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Gameplay/Attributes/Attributes.h"
#include "../Gameplay/Alteration/AlterationController.h"
#include "../UI/ELivingTextAnimMode.h"
#include "GameFramework/Character.h"
#include "Location.h"
#include "RobotRebellionCharacter.generated.h"


/*
 * Mother class for every character in RobotRebellion Game
 */
UCLASS(config = Game)
class ARobotRebellionCharacter : public ACharacter
{
    GENERATED_BODY()

private:
    bool m_isShieldAnimated;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
        float m_moveForwardSpeed;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
        float m_moveStraphSpeed;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 0.00001f))
        float m_maxVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 0.00001f))
        float m_maxCrouchVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 0.00001f))
        float m_maxWalkVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 0.00001f))
        float m_maxRunVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = 0.00001f, ClampMax = 1.f))
        float m_accelerationCoeff;


    ////Weapon Inventory/////
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
        class UWeaponInventory* m_weaponInventory;

    ////Billboard on character////
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BillBoard")
        TSubclassOf<class UTextBillboardComponent> m_textBillboardDefault;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BillBoard")
        class UTextBillboardComponent* m_textBillboardInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA")
        bool m_canKillItsAllies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA")
        bool m_canTransmitItsTarget;

    UPROPERTY(Replicated)
        bool m_isInCombat;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute, meta = (AllowPrivateAccess = "true"), Replicated)
        UAttributes* m_attribute;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute, meta = (AllowPrivateAccess = "true"))
        UAlterationController* m_alterationController;


    bool m_isInvisible;

    ////RESTORE MANA EFFECT
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RestoreMana)
        UParticleSystem* m_restoreManaParticuleEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RestoreMana)
        float m_restoreManaEffectDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RestoreMana)
        UParticleSystemComponent* m_restoreManaParticleSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RestoreMana)
        bool m_isRestoreManaParticleSpawned;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RestoreMana)
        float m_restoreManaEffectTimer;


    ////REVIVE EFFECT
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Revive)
        UParticleSystem* m_reviveParticuleEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Revive)
        float m_reviveEffectDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Revive)
        UParticleSystemComponent* m_reviveParticleSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Revive)
        bool m_isReviveParticleSpawned;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Revive)
        float m_reviveEffectTimer;



    ////SHIELD EFFECT
    /** Shield effect if animation shield animation is enabled*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shield)
        UParticleSystem* m_shieldParticuleEffect;

    /** Shield effect if animation shield animation is dsables*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shield)
        UParticleSystem* m_shieldParticuleEffectUnanimated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shield)
        UParticleSystemComponent* m_shieldParticleSystem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shield)
        bool m_isShieldParticleSpawned;

    UPROPERTY(BlueprintReadOnly, Replicated)
        int m_burningBonesCount;

    TArray<int32> m_burningBones;
    TArray<UParticleSystemComponent*> m_fireEffects;
    TMap<UParticleSystemComponent*, float> m_effectTimer;
    float m_tickCount;
    int m_bonesToUpdate;
    int m_bonesSet;


    class AWorldInstanceEntity* m_worldEntity;

    float m_decelerationCoeff;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
        ELocation m_location;

    /************************************************************************/
    /* PROPERTY                                                             */
    /************************************************************************/
    void(ARobotRebellionCharacter::* m_timedDestroyDelegate)(float deltaTime);
    void(ARobotRebellionCharacter::* m_disableBeforeDestroyDelegate)();


public:
    ////HEALTH BAR

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HealthBar)
       class UWidgetComponent* m_healthBar;

public:
    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/


    ARobotRebellionCharacter();


    bool hasDoubleWeapon() const USE_NOEXCEPT;

    class UWeaponBase* getCurrentEquippedWeapon() const USE_NOEXCEPT;

    const class UWeaponBase* getMainWeapon() const USE_NOEXCEPT;

    const class UWeaponBase* getSecondaryWeapon() const USE_NOEXCEPT;

    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;


    ////Server
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


    virtual void cppOnRevive();

    virtual void cppOnDeath();


    void startTimedDestroy() USE_NOEXCEPT;

    void inflictStun();
    void inflictStun(float duration);

    void inflictInvisibility();

    void addShield(float amount, float duration);

    void doesNothing()
    {}

    UTextBillboardComponent* getBillboardComponent();

    void updateIfInCombat();

    virtual FVector aim(const FVector& directionToShoot) const
    {
        return directionToShoot;
    }


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
    
    UFUNCTION(BlueprintNativeEvent, Category = "UpdateMethod")
        void updateInvisibilityMat(bool isVisible);

    UFUNCTION()
        void onDeath();

    UFUNCTION(Reliable, Client, WithValidation)
        void clientOnDeath();

    UFUNCTION(BlueprintCallable, Category = "Billboard - Living Text")
        void createTextBillboard();

    UFUNCTION(BlueprintCallable, Category = "Billboard - Living Text")
        void createTextBillboardWithThisCamera(UCameraComponent* camera);

    void setBillboardInstanceNewCamera(UCameraComponent* camera);

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

    UFUNCTION()
        bool isVisible() const;

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiSetInvisible(bool isInvisible);

    UFUNCTION()
        UAttributes* getAttributes()
    {
        return m_attribute;
    }


    // Attributs relatives functions added by macro
public:
    GENERATED_USING_AND_METHODS_FROM_Attributes(m_attribute, ->);

    UFUNCTION()
        void inflictDamage(float damage, ELivingTextAnimMode animType = ELivingTextAnimMode::TEXT_ANIM_MOVING, const FColor& damageColor = FColor::Red);

    UFUNCTION()
        void restoreHealth(float value, ELivingTextAnimMode animType = ELivingTextAnimMode::TEXT_ANIM_MOVING);

    UFUNCTION()
        void restoreMana(float value, ELivingTextAnimMode animType = ELivingTextAnimMode::TEXT_ANIM_MOVING);


    ////Restore Mana Effect
    UFUNCTION()
        void spawnManaParticle();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiSpawnManaParticle();


    UFUNCTION()
        void unspawnManaParticle();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiUnspawnManaParticle();


    ////Revive Effect

    UFUNCTION()
        void spawnReviveParticle();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiSpawnReviveParticle();


    UFUNCTION()
        void unspawnReviveParticle();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiUnspawnReviveParticle();


    ////Shield Effect

    UFUNCTION()
        void spawnShieldParticle();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiSpawnShieldParticle();

    UFUNCTION()
        void unspawnShieldParticle();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiUnspawnShieldParticle();


    ///////Burn Effects

    

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fire)
        UParticleSystem* m_fireEffect;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fire)
        class UParticleSystemComponent* m_particuleComponent;

    void UpdateBurnEffect(float DeltaTime);
    void displayFireOnBone(const FName& bone);

    UFUNCTION(Reliable, NetMulticast)
        void multiDisplayFireOnBone(const FName& bone);

    void internalDisplayFireOnBone(const FName& bone);

    void displayFireOnBoneArray(const TArray<FName>& bone);

    void spawnFireEffect(FVector location);

    UFUNCTION(Reliable, NetMulticast)
        void multiDisplayFireOnBoneArray(const TArray<FName>& bone);

    void internalDisplayFireOnBoneArray(const TArray<FName>& bone);
    
    bool isBurning() const USE_NOEXCEPT
    {
        return (m_burningBonesCount > 0);
    }
    
    UFUNCTION(Reliable, NetMulticast)
        void multiSpawnFireEffect(FVector location);


    UFUNCTION(Reliable, Server, WithValidation)
        void serverSpawnFireEffect(FVector location);

    void internalSpawnFireEffect(FVector location);

    void cleanFireComp();
    
    UFUNCTION(Reliable, NetMultiCast)
        void multiCleanFireComp();
    UFUNCTION(Reliable, Server, WithValidation)
        void serverCleanFireComp();

    void internalCleanFireComp();


    UFUNCTION(BlueprintCallable, Category = "Reverberation")
        ELocation GetLocation()
    {
        return m_location;
    }

    UFUNCTION(BlueprintCallable, Category = "Reverberation")
        void setLocation(ELocation location)
    {
        m_location = location;
    }
protected:
    template<class Alteration, class AdditionalFunc, class ... AdditionalArgs>
    void internalInflictAlteration(AdditionalFunc func, AdditionalArgs&& ... args)
    {
        Alteration* alteration;

        if(UUtilitaryFunctionLibrary::createObjectFromDefaultWithoutAttach<Alteration>(
            &alteration,
            *GameAlterationInstaller::getInstance().getAlterationDefault<Alteration>()
            ))
        {
            func(alteration, std::forward<AdditionalArgs>(args)...);
            m_alterationController->addAlteration(alteration);
        }

    }
};

