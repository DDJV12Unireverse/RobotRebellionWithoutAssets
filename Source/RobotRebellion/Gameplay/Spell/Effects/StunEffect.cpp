// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "StunEffect.h"

#include "Character/RobotRebellionCharacter.h"


void UStunEffect::BeginPlay()
{
    Super::BeginPlay();
}

void UStunEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStunEffect::exec(ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
{
    target->inflictStun(m_duration);
}

void UStunEffect::exec(const FVector impactPoint, ARobotRebellionCharacter* caster)
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
    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Blue, "Stun applyed on : " + FString::FromInt(hitActors.Num()) + " actors");
    for(FHitResult& currentHit : hitActors)
    {
        ARobotRebellionCharacter* temp = Cast<ARobotRebellionCharacter>(currentHit.GetActor());
        if(temp)
        {
            temp->inflictStun(m_duration);
        }
    }
}
