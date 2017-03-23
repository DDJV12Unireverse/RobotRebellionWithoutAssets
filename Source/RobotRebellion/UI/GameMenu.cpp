// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LobbyUIWidget.h"
//#include "ReviveTimerWidget.h"
#include "GameMenu.h"
#include "Character/PlayableCharacter.h"

AGameMenu::AGameMenu()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGameMenu::BeginPlay()
{
    Super::BeginPlay();

    HUDCharacterImpl = CreateCustomWidget<UCustomRobotRebellionUserWidget>(HUDCharacterWidget.GetDefaultObject());
    //HUDCharacterImpl->SetVisibility(ESlateVisibility::Hidden);

    LobbyImpl = CreateCustomWidget<ULobbyUIWidget>(Cast<ULobbyUIWidget>(LobbyWidget->GetDefaultObject()));
    LobbyImpl->initialiseOnliSubsystem();
    HideWidget(LobbyImpl);

    ReviveTimerWidgetImpl = CreateCustomWidget<UReviveTimerWidget>(ReviveWidget.GetDefaultObject());
    HideWidget(ReviveTimerWidgetImpl);
    //HUDCharacterImpl->SetVisibility(ESlateVisibility::Visible);
}
void AGameMenu::Tick(float delta)
{
    APlayableCharacter* player = Cast<APlayableCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
    if (player->isReviving())
    {
        DisplayWidget(ReviveTimerWidgetImpl);
    }
    else
    {
        HideWidget(ReviveTimerWidgetImpl);
    }
    
}