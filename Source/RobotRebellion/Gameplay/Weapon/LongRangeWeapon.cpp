// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LongRangeWeapon.h"
#include "Projectile.h"
#include "AudioDevice.h"

#include "../../Character/RobotRebellionCharacter.h"


ULongRangeWeapon::ULongRangeWeapon() :
    UWeaponBase()
{

}

void ULongRangeWeapon::cppAttack(ARobotRebellionCharacter* user)
{
    bool canFire = canAttack();
    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Cyan, "LongAtt");
    if(canFire && m_projectileClass != NULL)
    {
        // Retrieve the camera location and rotation
        FVector cameraLocation;
        FRotator muzzleRotation;
        user->GetActorEyesViewPoint(cameraLocation, muzzleRotation);

        // m_muzzleOffset is in camera space coordinate => must be transformed to world space coordinate.
        const FVector MuzzleLocation = cameraLocation + FTransform(muzzleRotation).TransformVector(m_muzzleOffset);
        //muzzleRotation.Pitch += LIFT_OFFSET; // lift the fire a little 
        UWorld* const World = user->GetWorld();
        if(World)
        {
            FActorSpawnParameters spawnParams;
            spawnParams.Owner = user;
            spawnParams.Instigator = user->Instigator;

            // spawn a projectile
            AProjectile* const projectile = World->SpawnActor<AProjectile>(
                m_projectileClass,
                MuzzleLocation,
                muzzleRotation,
                spawnParams
                );

            if(projectile)
            {
                projectile->setOwner(user);

                PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Purple, "FIRE");

                // Fire
                const FVector fireDirection = muzzleRotation.Vector();
                projectile->InitVelocity(fireDirection);

                playSound(m_longRangeWeaponFireSound, user);

                reload();
            }
        }
    }
    else if(!canFire)
    {
        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "Cannot attack !!! Reloading");
    }
    else
    {
        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Emerald, "Projectile null");
    }
}

FString ULongRangeWeapon::rangeToFString() const USE_NOEXCEPT
{
    return "Long Range weapon";
}

void ULongRangeWeapon::playSound(USoundCue* sound, AActor* originator)
{
    UAudioComponent* audioComponent = nullptr;
    if(sound && originator)
    {
        UGameplayStatics::PlaySoundAtLocation(originator, sound, originator->GetActorLocation());
    }
}