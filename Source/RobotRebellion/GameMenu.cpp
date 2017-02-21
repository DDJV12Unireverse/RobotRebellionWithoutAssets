// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LobbyUIWidget.h"
#include "GameMenu.h"

AGameMenu::AGameMenu()
{}

void AGameMenu::BeginPlay()
{
    Super::BeginPlay();

    LobbyImpl = CreateCustomWidget<ULobbyUIWidget>(Cast<ULobbyUIWidget>(LobbyWidget->GetDefaultObject()));
    HideWidget(LobbyImpl);

    HUDCharacterImpl = CreateCustomWidget<UCustomRobotRebellionUserWidget>(HUDCharacterWidget.GetDefaultObject());
    //HUDCharacterImpl->SetVisibility(ESlateVisibility::Hidden);
}
