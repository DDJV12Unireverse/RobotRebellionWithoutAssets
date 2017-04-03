// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RestoreManaEffect.h"

#include "Character/RobotRebellionCharacter.h"

void URestoreManaEffect::BeginPlay()
{
    Super::BeginPlay();
}

void URestoreManaEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URestoreManaEffect::exec(ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
{
    target->restoreMana(m_manaGiven);
}

void URestoreManaEffect::exec(const FVector impactPoint)
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
                                                                   EDrawDebugTrace::ForDuration,
                                                                   hitActors,
                                                                   true);
    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Blue, "Restore mana on : " + FString::FromInt(hitActors.Num()) + " actors");
    for(FHitResult& currentHit : hitActors)
    {
        ARobotRebellionCharacter* temp = Cast<ARobotRebellionCharacter>(currentHit.GetActor());
        if(temp)
        {
            temp->restoreMana(m_manaGiven);
        }
    }
}
