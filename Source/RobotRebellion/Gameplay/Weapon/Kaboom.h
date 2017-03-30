// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Kaboom.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API AKaboom : public AActor
{
	GENERATED_BODY()
	
	
public:
    UPROPERTY(VisibleDefaultsOnly, Category = "Bomb Attribute")
        USphereComponent* m_collisionComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge")
        class UStaticMeshComponent* m_kaboomMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
        UProjectileMovementComponent* m_kaboomMovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
        class UParticleSystemComponent* m_explosionPCS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
        FVector m_explosionEffectScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Attribute", meta = (ClampMin = 0.f))
        float m_baseDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb Attribute", meta = (ClampMin = 0.f))
        float m_detonationRadius;


protected:
    class ADrone* m_linkedDrone;

    TArray<TEnumAsByte<EObjectTypeQuery>> m_objectTypesToConsider;

    void (AKaboom::* m_activeBoomMethod)();
    void (AKaboom::* m_destroyMethod)();


public:
    AKaboom();
    virtual ~AKaboom() = default;

    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;


    FORCEINLINE UFUNCTION(BlueprintCallable, Category = "Action")
        void activateBomb()
    {
        m_activeBoomMethod = &AKaboom::detonationImplementation;
    }

    FORCEINLINE UFUNCTION(BlueprintCallable, Category = "Action")
        void deactivateBomb()
    {
        m_activeBoomMethod = &AKaboom::noMethod;
    }

    UFUNCTION(BlueprintCallable, Category = "Action")
        FORCEINLINE bool isActivated() const USE_NOEXCEPT 
    { 
        return m_activeBoomMethod == &AKaboom::detonationImplementation;
    }

    UFUNCTION(BlueprintCallable, Category = "Action")
        void attachToDrone(class ADrone* drone);

    UFUNCTION(BlueprintCallable, Category = "Action")
        void detachFromDrone();

    UFUNCTION()
        virtual void onHit(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent*
            OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
    {
        (this->*m_activeBoomMethod)();
    }

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void multiExplosionOnEveryone();


protected:
    void noMethod() {}

    void detonationImplementation();

    void realDestroy();


    void initializeDamagedObjectList();

    void initializeKaboomMovementComponent();

    void dropingPhysicSetting(bool reenablePhysic);
};
