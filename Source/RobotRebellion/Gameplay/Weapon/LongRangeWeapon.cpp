// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LongRangeWeapon.h"
#include "Projectile.h"

#include "AudioDevice.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/RobotRebellionCharacter.h"
#include "Character/PlayableCharacter.h"


ULongRangeWeapon::ULongRangeWeapon() :
    UWeaponBase()
{}

void ULongRangeWeapon::fireMethod(AProjectile* projectile, const FVector& fireDirection)
{
    if(projectile->isRaycast())
    {
        projectile->simulateInstant(fireDirection, m_WeaponRadiusRange);
    }
    else
    {
        projectile->InitProjectileParams(fireDirection, m_WeaponRadiusRange);
    }
}

void ULongRangeWeapon::cppAttack(ARobotRebellionCharacter* user)
{
    bool canFire = canAttack();
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

                FVector fireDirection = user->aim(muzzleRotation.Vector());
                USoundCue* sound = m_longRangeWeaponOutsideFireSound;
                // Fire
                fireMethod(projectile, fireDirection);
                APlayableCharacter* player = Cast<APlayableCharacter>(user);
                if(player)
                {
                    switch(player->GetLocation())
                    {
                        case ELocation::BIGROOM:
                            sound = m_longRangeWeaponBigRoomFireSound;
                            break;
                        case ELocation::CORRIDOR:
                            sound = m_longRangeWeaponCorridorFireSound;
                            break;
                        case ELocation::SMALLROOM:
                            sound = m_longRangeWeaponSmallRoomFireSound;
                            break;
                        default:
                            sound = m_longRangeWeaponOutsideFireSound;
                    }
                }
                playSound(sound, user);

                reload();
            }
        }
    }
}

void ULongRangeWeapon::cppAttack(ARobotRebellionCharacter* user, ARobotRebellionCharacter* ennemy)
{
    bool canFire = canAttack();
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

                // Fire
                const FVector fireDirection = user->aim(UKismetMathLibrary::GetForwardVector(
                    UKismetMathLibrary::FindLookAtRotation(user->GetActorLocation(), ennemy->GetActorLocation())));

                fireMethod(projectile, fireDirection);

                USoundCue* sound;
                // Fire
                fireMethod(projectile, fireDirection);
                
                    switch(user->GetLocation())
                    {
                        case ELocation::BIGROOM:
                            sound = m_longRangeWeaponBigRoomFireSound;
                            break;
                        case ELocation::CORRIDOR:
                            sound = m_longRangeWeaponCorridorFireSound;
                            break;
                        case ELocation::SMALLROOM:
                            sound = m_longRangeWeaponSmallRoomFireSound;
                            break;
                        default:
                            sound = m_longRangeWeaponOutsideFireSound;
                    }
                
                playSound(sound, user);

                reload();
            }
        }
    }
}

FString ULongRangeWeapon::rangeToFString() const USE_NOEXCEPT
{
    return "Long Range weapon";
}

void ULongRangeWeapon::playSound_Implementation(USoundCue* sound, AActor* originator)
{
    if(sound && originator)
    {
        UGameplayStatics::SpawnSoundAttached(sound, originator->GetRootComponent());
    }
}