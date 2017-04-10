// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "SphereCastSpell.h"

#include "Character/RobotRebellionCharacter.h"



USphereCastSpell::USphereCastSpell() : USpell()
{}

void USphereCastSpell::BeginPlay()
{
    Super::BeginPlay();
}

void USphereCastSpell::cast()
{
    if(!canCast())
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Emerald,
                                "Cooldown : " + FString::FromInt(m_nextAllowedCastTimer - FPlatformTime::Seconds()));
        return;
    }

    ARobotRebellionCharacter* caster = Cast<ARobotRebellionCharacter>(GetOwner());

    if(caster)
    {
        // Get player location and where hes looking
        FVector cameraLocation;
        FRotator muzzleRotation;
        caster->GetActorEyesViewPoint(cameraLocation, muzzleRotation);

        // Initialize Location & aim direction
        FVector aimDir = getRealAimingVector(caster);
        FVector baseEyeHeight = FVector(0.f, 0.f, caster->BaseEyeHeight);
        FVector endLocation = caster->GetActorLocation() + (aimDir * m_range) + baseEyeHeight;
        FVector startLocation = caster->GetActorLocation() + baseEyeHeight;
        // offset start location to fit with camera hight
        startLocation.Z = cameraLocation.Z;

        // proceed sphere cast
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)); // Players
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3)); // Robots
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4)); // Sovec
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6)); // Beast
        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(caster);
        TArray<FHitResult> hitActors;
        bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
                                                                       startLocation,
                                                                       endLocation,
                                                                       m_sphereRadius,
                                                                       ObjectTypes,
                                                                       false,
                                                                       ActorsToIgnore,
                                                                       EDrawDebugTrace::ForDuration,
                                                                       hitActors,
                                                                       true);

        for(FHitResult& currentHit : hitActors)
        {
            ARobotRebellionCharacter* tempCharacter = Cast<ARobotRebellionCharacter>(currentHit.GetActor());
            if(tempCharacter)
            {
                applyEffect(tempCharacter);
            }
        }
    }
}

void USphereCastSpell::applyEffect(ARobotRebellionCharacter* affectedTarget)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, TEXT("ApplicateEffect on target"));
    for(int i = 0; i < m_effects.Num(); ++i)
    {
        m_effects[i]->exec(Cast<ARobotRebellionCharacter>(GetOwner()), affectedTarget);
    }
}

void USphereCastSpell::applyEffect(FVector impactPoint)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, TEXT("ApplicateEffect on point"));
    for(int i = 0; i < m_effects.Num(); ++i)
    {
        m_effects[i]->exec(impactPoint);
    }
}

FVector USphereCastSpell::getRealAimingVector(const ARobotRebellionCharacter* caster)
{
    APlayerController* playerController = Cast<APlayerController>(caster->Controller);
    if(playerController)
    {
        FVector CamLoc;
        FRotator CamRot;
        playerController->GetPlayerViewPoint(CamLoc, CamRot);
        return CamRot.Vector();
    }
    else if(caster->Instigator)
    {
        return caster->GetBaseAimRotation().Vector();
    }
    return FVector::ZeroVector;
}
