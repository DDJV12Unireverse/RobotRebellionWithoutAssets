// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IA/Controller/EnnemiAIController.h"
#include "RobotShooterController.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ARobotShooterController : public AEnnemiAIController
{
	GENERATED_BODY()
private:
    FVector m_shootLocation;

public:
    // Debug use
    float m_detectionRange;

public:
    // specifie the distance to the target for the shoot position (percentage of weapon range)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot location", meta = (ClampMin = 0.f, ClampMax = 1.f))
        float m_distanceToShoot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes Height")
        float m_crouchEyesHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes Height")
        float m_standingEyesHeight;

public:

    void CheckEnnemyNear(float range) override;

    void AttackTarget() const override;

    /** Crouch the animation   */
    void crouch() const;

    // Uncrouch the pawn
    void uncrouch() const;

    /** return true if the pawn is crouch*/
    bool isCrouch() const;

    // Update shootposition
    void updateShootLocation();

    // Same as moveToTartget function but use shootLocation instead of target to follow
    EPathFollowingRequestResult::Type moveToShootLocation();

    //Draw debug on screen
    void drawDebug();
};
