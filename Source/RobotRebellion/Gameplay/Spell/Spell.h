// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Spell.generated.h"

/**
 * Spell Base class implement shared methode like initializeSpell and canCast
 * 
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API USpell : public UActorComponent
{
    GENERATED_BODY()
private:
    float m_realeaseInputTimeAfterCasting;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        FString m_name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        float m_range;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        float m_cooldown;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        float m_manaCost;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        bool m_hasMotionlessCastingTime;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        float m_castingTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
        TArray<TSubclassOf<class UEffect>> m_effectsClass;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute, meta = (AllowPrivateAccess = "true"))
        float m_nextAllowedCastTimer;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute, meta = (AllowPrivateAccess = "true"), Replicated)
        float m_currentCooldown;

    TArray<UEffect *> m_effects;

public:
    USpell();

    virtual void cast();

    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    // Must be called to initialize effect list with Blueprint specified class
    void initializeSpell();

    bool canCast() const;

    // return the actual time remaining before casting is possible if can cast return -1.f
    float getCurrentCooldown() const;

    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

};
