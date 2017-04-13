// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ShieldEffect.h"

void UShieldEffect::BeginPlay()
{
    Super::BeginPlay();
}

void UShieldEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShieldEffect::exec(ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
{
    // Inflict shield alteration on target
    target->addShield(m_shieldAmount, m_duration);
    PRINT_MESSAGE_ON_SCREEN(FColor::Magenta, "shielded for : " + FString::SanitizeFloat(m_shieldAmount)
                            + "during : " + FString::SanitizeFloat(m_duration) + "s");
}

void UShieldEffect::exec(const FVector& impactPoint, ARobotRebellionCharacter* caster)
{
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)); // Players
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3)); // Robots
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4)); // Sovec
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6)); // Beast
    TArray<AActor*> ActorsToIgnore{};
    TArray<FHitResult> hitActors;
    bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
                                                                   impactPoint,
                                                                   impactPoint,
                                                                   m_zoneRadius,
                                                                   ObjectTypes,
                                                                   false,
                                                                   ActorsToIgnore,
                                                                   SPHERECAST_DISPLAY_DURATION,
                                                                   hitActors,
                                                                   true);
    PRINT_MESSAGE_ON_SCREEN(FColor::Magenta, "shielded for : " + FString::SanitizeFloat(m_shieldAmount)
                            + "during : " + FString::SanitizeFloat(m_duration) + "s on :" 
                            + FString::FromInt(hitActors.Num()) + " target(s)");
    for(FHitResult& currentHit : hitActors)
    {
        ARobotRebellionCharacter* temp = Cast<ARobotRebellionCharacter>(currentHit.GetActor());
        if(temp)
        {
            // Apply shield effect
            temp->addShield(m_shieldAmount, m_duration);
        }
    }
}

