// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ThrowSpell.h"
#include "Gameplay/Spell/Effects/ProjectileEffect.h"
#include "Gameplay/Spell/Effects/Effect.h"
#include "Character/RobotRebellionCharacter.h"

UThrowSpell::UThrowSpell() : USpell()
{}

void UThrowSpell::BeginPlay()
{
    Super::BeginPlay();
}

void UThrowSpell::cast()
{
    if(!canCast())
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Emerald,
                                "Cooldown : " + FString::FromInt(m_nextAllowedCastTimer -FPlatformTime::Seconds()));
        return;
    }

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

        // spawn the effect projectile
        AProjectileEffect* const projectile = world->SpawnActor<AProjectileEffect>(
            m_projectileClass,
            MuzzleLocation,
            muzzleRotation,
            spawnParams
            );

        if(projectile)
        {
            const FVector fireDirection = muzzleRotation.Vector();
            projectile->setOwner(caster);
            projectile->setParent(this);
            projectile->initMovement(fireDirection);

            // the spell is successfully cast consumme mana and launch CD
            caster->consumeMana(m_manaCost);
            m_nextAllowedCastTimer = FPlatformTime::Seconds() + m_cooldown;
        }
    }
}

void UThrowSpell::onHit(UPrimitiveComponent*, AActor* target, UPrimitiveComponent*, FVector, const FHitResult& hitResult)
{
    if(m_isTargetThrow && target != this->GetOwner())
    {
        ARobotRebellionCharacter* hitChar = Cast<ARobotRebellionCharacter>(target);
        if(hitChar)
        {
            applyEffect(hitChar);
        }
    }
    else
    {
        applyEffect(hitResult.ImpactPoint);
    }
}

void UThrowSpell::applyEffect(ARobotRebellionCharacter* affectedTarget)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, TEXT("ApplicateEffect on target"));
    for(int i = 0; i < m_effects.Num(); ++i)
    {
        m_effects[i]->exec(Cast<ARobotRebellionCharacter>(GetOwner()), affectedTarget);
    }
}

void UThrowSpell::applyEffect(FVector impactPoint)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, TEXT("ApplicateEffect on point"));
    for(int i = 0; i < m_effects.Num(); ++i)
    {
        m_effects[i]->exec(impactPoint);
    }
}