// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Effect.generated.h"

/*
 * Interface class for all effects
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

public:
    // Sets default values for this component's properties
    UEffect();

    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // The behavior of the effect when it's a targeted effect
    virtual void exec(class ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target) PURE_VIRTUAL(UEffect::exec, );
    // The behavior of the effect when it's point effect
    virtual void exec(const FVector& impactPoint, ARobotRebellionCharacter* caster = nullptr) PURE_VIRTUAL(UEffect::exec, );
};
