// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Effect.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTREBELLION_API UEffect : public UActorComponent
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
        float m_zoneRadius;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
        float m_duration;
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
    //    ANIMATION m_animation;


	// Sets default values for this component's properties
	UEffect();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    // The behavior of the effect (spawn the grenade, launch a raycast, etc.)
    virtual void exec() PURE_VIRTUAL(UEffect::exec, );

    // When the effect touch something
    virtual void onHit(class UPrimitiveComponent*, class AActor*, class UPrimitiveComponent*, FVector, const FHitResult&) PURE_VIRTUAL(UEffect::onHit, );
	
};
