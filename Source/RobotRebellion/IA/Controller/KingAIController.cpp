// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "KingAIController.h"
#include "Global/EntityDataSingleton.h"
#include "DroneAIController.h"


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
    EntityDataSingleton& datas = EntityDataSingleton::getInstance();

    ADroneAIController* droneController = Cast<ADroneAIController>(datas.m_drone->GetController());

    ensure(droneController);
    
    m_destination = droneController->getAllyBarycenter();
}