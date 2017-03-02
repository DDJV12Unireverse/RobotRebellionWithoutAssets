// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ThrowSpell.h"
#include "RobotRebellionCharacter.h"
#include "ProjectileEffect.h"

void UThrowSpell::cast()
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, TEXT("Cast Throw Spell"));
    ARobotRebellionCharacter* caster = Cast<ARobotRebellionCharacter>(GetOwner());
    UWorld* const world = caster->GetWorld();

    if(caster)
    {
        FVector cameraLocation;
        FRotator muzzleRotation;
        caster->GetActorEyesViewPoint(cameraLocation, muzzleRotation);

        const FVector MuzzleLocation = cameraLocation + FTransform(muzzleRotation).TransformVector(m_muzzleOffset);
        muzzleRotation.Pitch += m_liftOffset;

        FActorSpawnParameters spawnParams;
        spawnParams.Owner = caster;
        spawnParams.Instigator = caster->Instigator;

        // spawn a projectile
        AProjectileEffect* const projectile = world->SpawnActor<AProjectileEffect>(
            m_projectileClass,
            MuzzleLocation,
            muzzleRotation,
            spawnParams
            );

        if(projectile)
        {
            projectile->setOwner(caster);
            PRINT_MESSAGE_TO_TEST_OBJECT_NULLITY(Cast<USpell>(this), FColor::Emerald);
            projectile->setParent(this);
            const FVector fireDirection = muzzleRotation.Vector();
            projectile->initMovement(fireDirection, m_projectileInitialSpeed);
        }
    }
}

void UThrowSpell::onHit(UPrimitiveComponent*, AActor* target, UPrimitiveComponent*, FVector, const FHitResult& hitResult)
{
    if(m_isTargetThrow)
    {
        ARobotRebellionCharacter* hitChar = Cast<ARobotRebellionCharacter>(target);
        if(hitChar)
        {
            applicateEffect(hitChar);
        }
    }
    else
    {
        applicateEffect(hitResult.ImpactPoint);
    }
}

void UThrowSpell::applicateEffect(ARobotRebellionCharacter* affectedTarget)
{
}

void UThrowSpell::applicateEffect(FVector impactPoint)
{
}
