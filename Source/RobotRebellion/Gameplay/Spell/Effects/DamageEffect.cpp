// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "DamageEffect.h"

//#include "../../../Damage/Damage.h"
//#include "../../../Tool/Algorithm.h"
#include "Character/RobotRebellionCharacter.h"

void UDamageEffect::BeginPlay()
{
    Super::BeginPlay();
}

void UDamageEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDamageEffect::exec(class ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
{
    inflictEffectDamage(target, caster);
}

void UDamageEffect::exec(const FVector impactPoint, ARobotRebellionCharacter* caster)
{
    //Considered Actors
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)); // Players
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3)); // Robots
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4)); // Sovec
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6)); // Beast

    //Ignored actors, only user
    TArray<AActor*> ActorsToIgnore;

    //Result
    TArray<FHitResult> OutHits;

    if(UKismetSystemLibrary::SphereTraceMultiForObjects(
        caster->GetWorld(),
        impactPoint,
        impactPoint,
        m_zoneRadius,
        ObjectTypes,
        false,
        ActorsToIgnore,
        SPHERECAST_DISPLAY_NONE,
        OutHits,
        true
    ))
    {
        ARobotRebellionCharacter** exReceiver = nullptr;
        int32 outCount = OutHits.Num();

        for(int32 noEnnemy = 0; noEnnemy < outCount; ++noEnnemy)
        {
            FHitResult hit = OutHits[noEnnemy];
            ARobotRebellionCharacter* receiver = Cast<ARobotRebellionCharacter>(hit.GetActor());
            if(receiver && exReceiver != &receiver && !receiver->isDead())
            {
                if(!receiver->isImmortal())
                {
                    inflictEffectDamage(receiver, caster);
                }
            }
        }
    }
}

void UDamageEffect::inflictEffectDamage(ARobotRebellionCharacter* target, class ARobotRebellionCharacter* caster)
{
    // Very simple way to deals damage
    float damage = m_flatDamage;
    damage += target->getMaxHealth() * m_hpPercent;
    float coef = caster->getStrength() / target->getDefense();
    coef = coef > 1.0f ? 1.0f : coef;
    damage += coef * m_reducedDamage;

    target->inflictDamage(damage);
}

