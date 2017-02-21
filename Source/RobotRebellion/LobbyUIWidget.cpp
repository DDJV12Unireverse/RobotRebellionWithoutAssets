// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotRebellionCharacter.h"
#include "LobbyUIWidget.h"

void ULobbyUIWidget::CreateServer(FString mapName)
{
    auto currentCharacter = Cast<ARobotRebellionCharacter>(GetOwningPlayer()->GetCharacter());

    FString command = "open " + mapName + "?listen";
    
    currentCharacter->ExecuteCommand(command);
}

void ULobbyUIWidget::JoinServer(FString IPAdress)
{
    auto currentCharacter = Cast<ARobotRebellionCharacter>(GetOwningPlayer()->GetCharacter());

    FString command = "open " + IPAdress + ":7777";

    currentCharacter->ExecuteCommand(command);
}

