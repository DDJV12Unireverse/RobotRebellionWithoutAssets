// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotRebellionCharacter.h"
#include "LobbyUIWidget.h"

void ULobbyUIWidget::CreateServer(ARobotRebellionCharacter* currentCharac, FString mapName)
{
    FString command = "open " + mapName + "?listen";
    
    currentCharac->ExecuteCommand(command);
}

void ULobbyUIWidget::JoinServer(ARobotRebellionCharacter* currentCharac, FString IPAdress)
{
    FString command = "open " + IPAdress + ":7777";

    currentCharac->ExecuteCommand(command);
}

