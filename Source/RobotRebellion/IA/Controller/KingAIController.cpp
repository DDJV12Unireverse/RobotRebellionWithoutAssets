// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "KingAIController.h"
#include "Global/EntityDataSingleton.h"
#include "DroneAIController.h"
#include "Tool/UtilitaryFunctionLibrary.h"
#include "Character/Drone.h"


void AKingAIController::BeginPlay()
{
    Super::BeginPlay();

    this->m_updateMethodPtr = &AKingAIController::doesNothing;
}

void AKingAIController::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    (this->*m_updateMethodPtr)();
}

void AKingAIController::updateKing()
{
    this->computeTarget();

    this->MoveToTarget();
}

EPathFollowingRequestResult::Type AKingAIController::MoveToTarget()
{
    return MoveToLocation(m_destination, m_moveThreshold);
}

void AKingAIController::computeTarget()
{
    ADrone* drone = EntityDataSingleton::getInstance().getServerDrone(this);

    //always good. Never nullptr because AKingAIController only lives on server side.
    if (drone)
    {
        ADroneAIController* droneController = Cast<ADroneAIController>(drone->GetController());

        check(droneController);

        if(FVector::DistSquared(m_destination, droneController->getAllyBarycenter()) > m_minimaleDistanceToMove)
        {
            m_destination = droneController->getAllyBarycenter();
        }
    }
}