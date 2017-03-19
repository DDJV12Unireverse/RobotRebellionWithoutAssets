// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RayCastSpell.h"
#include "Effect.h"
#include "../../Character/RobotRebellionCharacter.h"



URayCastSpell::URayCastSpell() : USpell()
{}

void URayCastSpell::BeginPlay()
{
    Super::BeginPlay();
}

void URayCastSpell::cast()
{
    if(!canCast())
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Emerald,
                                "Cooldown : " + FString::FromInt(m_nextAllowedCastTimer - FPlatformTime::Seconds()));
        return;
    }

    ARobotRebellionCharacter* caster = Cast<ARobotRebellionCharacter>(GetOwner());
    UWorld* const world = caster->GetWorld();

    if(caster)
    {
        // TODO - perform ray cast.
        // Get player location and where hes looking
        FVector cameraLocation;
        FRotator muzzleRotation;
        caster->GetActorEyesViewPoint(cameraLocation, muzzleRotation);
        //DrawDebugLine(world)

        // Initialize Location
        const FVector endLocation = cameraLocation + (muzzleRotation.Vector() * m_range);
        const FVector startLocation = caster->GetActorLocation();

        // Cast the RAY!
        FHitResult hitActors(ForceInit);
        FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, caster->Instigator);
        TraceParams.bTraceAsyncScene = true;
        TraceParams.bReturnPhysicalMaterial = true;
        // atm only should only proc on static mesh
        world->LineTraceSingleByChannel(hitActors, cameraLocation, endLocation, ECC_WorldStatic, TraceParams);
        // hit Actors countains hit actors now
        processHitActor(hitActors);
    }
}

void URayCastSpell::applyEffect(ARobotRebellionCharacter* affectedTarget)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, TEXT("ApplicateEffect on target"));
    for(int i = 0; i < m_effects.Num(); ++i)
    {
        m_effects[i]->exec(Cast<ARobotRebellionCharacter>(GetOwner()), affectedTarget);
    }
}

void URayCastSpell::applyEffect(FVector impactPoint)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, TEXT("ApplicateEffect on point"));
    for(int i = 0; i < m_effects.Num(); ++i)
    {
        m_effects[i]->exec(impactPoint);
    }
}

void URayCastSpell::processHitActor(const FHitResult& hitResult)
{
    if(hitResult.GetActor() != nullptr)
    {
        if(m_isTargetedSpell)
        {
            // apply effect on character hit
            ARobotRebellionCharacter* hitCharacter = Cast<ARobotRebellionCharacter>(hitResult.GetActor());
            if(hitCharacter)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Ray cast spell done -> apply effect on target");
                applyEffect(hitCharacter);
            }
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Ray cast spell done -> apply effect on impactpoint");
            applyEffect(hitResult.ImpactPoint);
        }
    }
}
