// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "NoHitRayCastSpell.h"
#include "Gameplay/Spell/Effects/Effect.h"
#include "Character/RobotRebellionCharacter.h"



UNoHitRayCastSpell::UNoHitRayCastSpell() : USpell()
{}

void UNoHitRayCastSpell::BeginPlay()
{
    Super::BeginPlay();
}

void UNoHitRayCastSpell::cast()
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
        // Get player location and where hes looking
        FVector cameraLocation;
        FRotator muzzleRotation;
        caster->GetActorEyesViewPoint(cameraLocation, muzzleRotation);

        FVector aimDir = getRealAimingVector(caster);

        // Initialize Location
        FVector endLocation = caster->GetActorLocation() + (aimDir * m_range);
        // offset the shoot to avoid collision with the capsule of the player
        FVector startLocation = caster->GetActorLocation() + (aimDir * 100.f) + FVector(0.f, 0.f, caster->BaseEyeHeight);
        //Draw debug line
        DRAW_DEBUG_LINE(world, startLocation, endLocation, FColor::Red);

        // Cast the RAY!
        FHitResult hitActors(ForceInit);
        FCollisionQueryParams TraceParams(TEXT("No hit raycast spell"), true, caster->Instigator);
        TraceParams.bTraceAsyncScene = true;
        TraceParams.bReturnPhysicalMaterial = true;
        // atm only should only proc on static mesh
        world->LineTraceSingleByChannel(hitActors, startLocation, endLocation, ECC_WorldStatic, TraceParams);
        // hit Actors countains hit actors now
        if(hitActors.GetActor() == nullptr)
        {
            // We've hit nothing
            // check if endlocation has engouh height
            float casterHalfHeight = caster->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
            float offsetHeight = 2.f;
            FVector endTestLocation = endLocation - FVector{0.f, 0.f,casterHalfHeight + offsetHeight};
            world->LineTraceSingleByChannel(hitActors, endLocation, endTestLocation, ECC_WorldStatic, TraceParams);
            if(hitActors.GetActor() != nullptr)
            {
                // We've hit something
                TArray<FHitResult> testHeightHits{};
                const FVector& impactPosition = hitActors.ImpactPoint;
                // let enough space below the character
                endLocation.Z = endLocation.Z + (impactPosition.Z - endTestLocation.Z);
            }
            applyEffect(endLocation);
            // TODO - check for top collisions
        }
        else
        {
            // is it a player?
            ARobotRebellionCharacter* hitCharacter = Cast<ARobotRebellionCharacter>(hitActors.GetActor());
            if(hitCharacter)
            {
                applyEffect(hitCharacter);
            }
            else
            {
                // have hit decors
                FVector impactPoint = hitActors.ImpactPoint;
                float capsuleRadius = caster->GetCapsuleComponent()->GetScaledCapsuleRadius();

                // offset endpoint based on capsule radius
                endLocation = impactPoint - (aimDir * capsuleRadius);

                // check if endlocation has engouh height
                float casterHalfHeight = caster->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
                float offsetHeight = 2.f;
                FVector endTestLocation = endLocation - FVector{0.f, 0.f,casterHalfHeight + offsetHeight};
                world->LineTraceSingleByChannel(hitActors, endLocation, endTestLocation, ECC_WorldStatic, TraceParams);
                if(hitActors.GetActor() != nullptr)
                {
                    // We've hit something
                    TArray<FHitResult> testHeightHits{};
                    const FVector& impactPosition = hitActors.ImpactPoint;
                    // let enough space below the character
                    endLocation.Z = endLocation.Z + (impactPosition.Z - endTestLocation.Z);
                }
                applyEffect(endLocation);
                // TODO - check for top collisions
            }
        }


        // the spell is successfully cast consumme mana and launch CD
        caster->consumeMana(m_manaCost);
        m_nextAllowedCastTimer = FPlatformTime::Seconds() + m_cooldown;
    }
}

void UNoHitRayCastSpell::applyEffect(ARobotRebellionCharacter* affectedTarget)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, TEXT("ApplicateEffect on target"));
    for(int i = 0; i < m_effects.Num(); ++i)
    {
        m_effects[i]->exec(Cast<ARobotRebellionCharacter>(GetOwner()), affectedTarget);
    }
}

void UNoHitRayCastSpell::applyEffect(FVector impactPoint)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, TEXT("ApplicateEffect on point"));
    for(int i = 0; i < m_effects.Num(); ++i)
    {
        m_effects[i]->exec(impactPoint, Cast<ARobotRebellionCharacter>(GetOwner()));
    }
}

FVector UNoHitRayCastSpell::getRealAimingVector(const ARobotRebellionCharacter* caster)
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