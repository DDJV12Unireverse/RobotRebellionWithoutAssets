// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CustomPlayerController.h"

void ACustomPlayerController::setInputMode_Implementation(bool status)
{
    if(status)
    {
        FInputModeGameOnly Mode;
        bShowMouseCursor = false;
        SetInputMode(Mode);
    }
    else
    {
        FInputModeGameAndUI Mode;
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
        Mode.SetHideCursorDuringCapture(false);
        bShowMouseCursor = true;
        SetInputMode(Mode);
    }
}

void ACustomPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void ACustomPlayerController::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

}

void ACustomPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


