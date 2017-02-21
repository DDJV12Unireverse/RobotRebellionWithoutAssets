// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CustomPlayerController.h"

#include "Blueprint/UserWidget.h"


void ACustomPlayerController::initializeHUD()
{
    if (m_HUDWidgets) // Check if the asset is well assigned in the blueprint.
    {
        // Create the widget HUD and store it into the container m_HUDContainer.
        m_HUDContainer = CreateWidget<UUserWidget>(this, m_HUDWidgets);

        if (m_HUDContainer)
        {
            m_HUDContainer->AddToViewport();
        }

        bShowMouseCursor = false;
    }
}
