// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotShooterController.h"

#include "Character/NonPlayableCharacter.h"
#include "Gameplay/Weapon/WeaponBase.h"




void ARobotShooterController::CheckEnnemyNear(float range)
{
    //debug use
    m_detectionRange = range;

    DrawDebugSphere(GetWorld(),
                    GetPawn()->GetActorLocation(),
                    range,
                    32,
                    FColor::Cyan,
                    false,
                    5.f, 0, 0.5f);

    ANonPlayableCharacter* ennemiCharacter = Cast<ANonPlayableCharacter>(GetCharacter());
    float weaponRange = ennemiCharacter->m_weaponInventory->getCurrentWeapon()->m_WeaponRadiusRange;
    DrawDebugSphere(GetWorld(),
                    GetPawn()->GetActorLocation(),
                    weaponRange,
                    32,
                    FColor::Red,
                    false,
                    5.f, 0, 0.5f);
    AEnnemiAIController::CheckEnnemyNear(range);
}

void ARobotShooterController::AttackTarget() const
{
    AEnnemiAIController::AttackTarget();
}

bool ARobotShooterController::isCrouch() const
{
    // TODO - return m_crouch
    ANonPlayableCharacter* ennemiCharacter = Cast<ANonPlayableCharacter>(GetCharacter());
    return ennemiCharacter->m_isCrouch;
    
}

void ARobotShooterController::crouch() const
{
    // TODO - Crouch the pawn if necessary
    ANonPlayableCharacter* ennemiCharacter = Cast<ANonPlayableCharacter>(GetCharacter());
    ennemiCharacter->m_isCrouch = true;
}

void ARobotShooterController::uncrouch() const
{
    // TODO - Uncrouch the pawn if necessary
    ANonPlayableCharacter* ennemiCharacter = Cast<ANonPlayableCharacter>(GetCharacter());
    ennemiCharacter->m_isCrouch = false;
}

void ARobotShooterController::updateShootLocation()
{
    // Get target Location
    const FVector& targetLoc = getTargetToFollowLocation();
    const FVector& pawnLoc = GetPawn()->GetActorLocation();

    // get direction vector from target to pawn
    FVector direction = pawnLoc - targetLoc;
    direction.Normalize();

    // process Distance from the target
    ANonPlayableCharacter* ennemiCharacter = Cast<ANonPlayableCharacter>(GetCharacter());
    float distanceToShoot = m_distanceToShoot *
        ennemiCharacter->m_weaponInventory->getCurrentWeapon()->m_WeaponRadiusRange;

    m_shootLocation = targetLoc + distanceToShoot * direction;

    DrawDebugSphere(GetWorld(),
                    m_shootLocation,
                    5.f,
                    32,
                    FColor::Blue,
                    false,
                    5.f, 0, 5.f);
}

EPathFollowingRequestResult::Type ARobotShooterController::moveToShootLocation()
{
    EPathFollowingRequestResult::Type MoveToActorResult = MoveToLocation(m_shootLocation);

    return MoveToActorResult;
}

// DEBUG
void ARobotShooterController::drawDebug()
{
    DrawDebugSphere(GetWorld(),
                    GetPawn()->GetActorLocation(),
                    m_detectionRange,
                    32,
                    FColor::Cyan,
                    false,
                    2.f, 0, 5.f);

    ANonPlayableCharacter* ennemiCharacter = Cast<ANonPlayableCharacter>(GetCharacter());
    float weaponRange = ennemiCharacter->m_weaponInventory->getCurrentWeapon()->m_WeaponRadiusRange;
    DrawDebugSphere(GetWorld(),
                    GetPawn()->GetActorLocation(),
                    weaponRange,
                    32,
                    FColor::Red,
                    false,
                    2.f, 0, 5.f);

    FColor positionColor;
    if(isCrouch())
    {
        positionColor = FColor::Red;
    }
    else
    {
        positionColor = FColor::Blue;
    }
    DrawDebugSphere(GetWorld(),
                    GetPawn()->GetActorLocation() + FVector{0.f, 0.f, 100.f},
                    5.f,
                    12,
                    positionColor,
                    false,
                    2.f, 0, 5.f);
}