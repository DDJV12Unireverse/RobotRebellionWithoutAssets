// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LobbyUIWidget.h"
//#include "ReviveTimerWidget.h"
#include "GameMenu.h"

AGameMenu::AGameMenu()
{}

void AGameMenu::BeginPlay()
{
    Super::BeginPlay();

    HUDCharacterImpl = CreateCustomWidget<UCustomRobotRebellionUserWidget>(HUDCharacterWidget.GetDefaultObject());
    //HUDCharacterImpl->SetVisibility(ESlateVisibility::Hidden);

    LobbyImpl = CreateCustomWidget<ULobbyUIWidget>(Cast<ULobbyUIWidget>(LobbyWidget->GetDefaultObject()));
    LobbyImpl->initialiseOnliSubsystem();
    HideWidget(LobbyImpl);

    ReviveTimerWidgetImpl = CreateCustomWidget<UReviveTimerWidget>(ReviveWidget.GetDefaultObject());
    HUDCharacterImpl->SetVisibility(ESlateVisibility::Visible);
}
