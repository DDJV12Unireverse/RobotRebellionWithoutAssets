// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WizardMeteor.generated.h"

/*
 * Actor invoke by wizard Ultimate, it must have lifespan
 * once the end of lifespan is reach it destroye him self and deals damage to every character hit
 */
UCLASS()
class ROBOTREBELLION_API AWizardMeteor : public AActor
{
	GENERATED_BODY()
	
private:
    float m_unreducedDamage;

public:
    /** Movement component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
        UProjectileMovementComponent* m_projectileMovement;

public:	
	// Sets default values for this actor's properties
	AWizardMeteor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    // Replication method
    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

private:
    // Inflict damage then destroye it self
    void explode();
	
};
