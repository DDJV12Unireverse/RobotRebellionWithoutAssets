// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LobbyUIWidget.h"
#include "GameMenu.h"

AGameMenu::AGameMenu()
{}

void AGameMenu::BeginPlay()
{
    Super::BeginPlay();

    //DisplayWidget<ULobbyUIWidget>(Cast<ULobbyUIWidget>(LobbyWidget->GetDefaultObject()));
}
