// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "TeleportationEffect.h"

#include "Character/RobotRebellionCharacter.h"

void UTeleportationEffect::BeginPlay()
{
    Super::BeginPlay();
}

void UTeleportationEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTeleportationEffect::exec(ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
{
    // Get new position
    FVector targetLoc = target->GetActorLocation();
    FVector targetFrontVector = target->GetActorForwardVector();

    // Process offset base on the max collision box extent between x and y (dont bother with height) of both actor
    FVector targetCollisionCylinder = target->GetSimpleCollisionCylinderExtent();
    float offset = targetCollisionCylinder.X > targetCollisionCylinder.Y ? targetCollisionCylinder.X : targetCollisionCylinder.Y;
    FVector casterCollisionCylinder = target->GetSimpleCollisionCylinderExtent();
    offset += casterCollisionCylinder.X > casterCollisionCylinder.Y ? casterCollisionCylinder.X : casterCollisionCylinder.Y;
    FVector teleportationLoc = targetLoc + (offset * (-targetFrontVector));

    // Get new rotation (the caster shouold face the target back) -> target frontVector Quat 
    FQuat newRotation = target->GetActorForwardVector().Rotation().Quaternion();

    // Inverse front vector and add offset
    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Emerald, 
        "targetPos = " + targetLoc.ToString() + " TeleportLocation = " + teleportationLoc.ToString());

    caster->SetActorLocation(teleportationLoc);
    caster->SetActorRotation(newRotation, ETeleportType::TeleportPhysics);
}

void UTeleportationEffect::exec(const FVector impactPoint)
{
    // No one hit, just go to the "impact point"

}

