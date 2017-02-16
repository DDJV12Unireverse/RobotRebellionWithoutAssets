// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class ROBOTREBELLION_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
    //////////////////////////////////////////////////////////////////////////ADDED MEMBERS /////
    
    /** Movement component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
        UProjectileMovementComponent* m_projectileMovement;

    // Collision Component
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
        USphereComponent* m_collisionComp;

    
    /** Initialize velocity */
    void InitVelocity(const FVector& shootDirection);

    //ON HIT
   /* UFUNCTION()
        void OnHit(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent*
            OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/
            ////Server
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


};
