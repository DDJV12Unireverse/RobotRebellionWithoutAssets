// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LongRangeWeapon.h"
#include "RobotRebellionCharacter.h"
#include "Projectile.h"


ULongRangeWeapon::ULongRangeWeapon() :
    UWeaponBase()
{
    
}

void ULongRangeWeapon::cppAttack(ARobotRebellionCharacter* user)
{
    bool canFire = canAttack();
    PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, "LongAtt");
    if (canFire && m_projectileClass != NULL)
    {
        // Retrieve the camera location and rotation
        FVector cameraLocation;
        FRotator muzzleRotation;
        user->GetActorEyesViewPoint(cameraLocation, muzzleRotation);

        // m_muzzleOffset is in camera space coordinate => must be transformed to world space coordinate.
        const FVector MuzzleLocation = cameraLocation + FTransform(muzzleRotation).TransformVector(m_muzzleOffset);
        muzzleRotation.Pitch += LIFT_OFFSET; // lift the fire a little 
        UWorld* const World = user->GetWorld();
        if (World)
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

            if (projectile)
            {
                projectile->setOwner(user);

                PRINT_MESSAGE_ON_SCREEN(FColor::Purple, "FIRE");

                // Fire
                const FVector fireDirection = muzzleRotation.Vector();
                projectile->InitVelocity(fireDirection);

                reload();
            }
        }
    }
    else if (!canFire)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Red, "Cannot attack !!! Reloading")
    }
}