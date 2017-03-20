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
        // Get player location and where hes looking
        FVector cameraLocation;
        FRotator muzzleRotation;
        caster->GetActorEyesViewPoint(cameraLocation, muzzleRotation);
        
        FVector aimDir = getRealAimingVector(caster);

        // Initialize Location
        const FVector endLocation = caster->GetActorLocation() + (aimDir * m_range);
        // offset the shoot to avoid collision with the capsule of the player
        const FVector startLocation = caster->GetActorLocation() + (aimDir * 100.f); 
        //Draw debug line
        DrawDebugLine(world, startLocation, endLocation, FColor::Red, false, 5.f);

        // Cast the RAY!
        FHitResult hitActors(ForceInit);
        FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, caster->Instigator);
        TraceParams.bTraceAsyncScene = true;
        TraceParams.bReturnPhysicalMaterial = true;
        // atm only should only proc on static mesh
        world->LineTraceSingleByChannel(hitActors, startLocation, endLocation, ECC_WorldStatic, TraceParams);
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

// void processStartLocAndAimVector(const FVector &camLoc, const FVector &camDir, const FVector &playerLoc,
//                                  FVector &startLoc, FVector &aimDirection)
// {
//     FVector camToPlayer = playerLoc - camLoc;
//     camToPlayer.Normalize();
//     camDir.Normalize();
// }

FVector URayCastSpell::getRealAimingVector(const ARobotRebellionCharacter* caster)
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
