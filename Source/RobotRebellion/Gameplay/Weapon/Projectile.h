// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class ARobotRebellionCharacter;

UCLASS()
class ROBOTREBELLION_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
    //////////////////////////////////////////////////////////////////////////ADDED MEMBERS /////
    

    ////OWNER////
    UPROPERTY(Transient, ReplicatedUsing = OnRep_MyOwner)
        ARobotRebellionCharacter *  m_owner;

    /** Movement component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
        UProjectileMovementComponent* m_projectileMovement;

    //// Collision ////
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
        USphereComponent* m_collisionComp;
	

public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void OnRep_MyOwner();

    void setOwner(ARobotRebellionCharacter *newOwner);

    // On hit function called every collision
    UFUNCTION()
        virtual void OnHit(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent*
            OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    void setReceiverInCombat(ARobotRebellionCharacter* receiver);

    
    /** Initialize velocity */
    virtual void InitProjectileParams(const FVector& shootDirection, float distanceRange);

    //ON HIT
   /* UFUNCTION()
        void OnHit(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent*
            OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/
            ////Server
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    void inflictDamageLogic(class AActor* OtherActor, const FHitResult& Hit);

    void simulateInstant(const FVector& shootDirection, float distance);

    void simulateInstantRealMethod(const FVector& shootDirection, float distanceRange);

    UFUNCTION(Reliable, Server, WithValidation)
        void serverSimulateInstant(const FVector& shootDirection, float distanceRange);

    UFUNCTION(NetMulticast, Reliable)
        void multiDrawLineOnClients(const FVector& start, const FVector& end);

    void drawProjectileLineMethod(UWorld* world, const FVector& start, const FVector& end);

    void suicide();

    UFUNCTION(NetMulticast, Reliable)
        void destroyOnClients();

    FORCEINLINE virtual bool isRaycast() const USE_NOEXCEPT
    {
        return false;
    }
};
