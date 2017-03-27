// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LobbyUIWidget.h"
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
    //HideWidget(HUDCharacterImpl);

    LobbyImpl = CreateCustomWidget<ULobbyUIWidget>(Cast<ULobbyUIWidget>(LobbyWidget->GetDefaultObject()));
    LobbyImpl->initialiseOnliSubsystem();
    LobbyImpl->initializeOwner();
    LobbyImpl->SetVisibility(ESlateVisibility::Hidden);

    ReviveTimerWidgetImpl = CreateCustomWidget<UReviveTimerWidget>(ReviveWidget.GetDefaultObject());
    ReviveTimerWidgetImpl->initializeOwner();
    ReviveTimerWidgetImpl->SetVisibility(ESlateVisibility::Hidden);
}

void AGameMenu::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    APlayableCharacter* player = Cast<APlayableCharacter>(GetOwningPlayerController()->GetCharacter());
    if(player->isReviving())
    {
        ReviveTimerWidgetImpl->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        ReviveTimerWidgetImpl->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AGameMenu::DisplayWidget(URobotRebellionWidget* WidgetRef)
{
    WidgetRef->SetVisibility(ESlateVisibility::Visible);
    WidgetRef->startSound();
}

void AGameMenu::HideWidget(URobotRebellionWidget* WidgetRef)
{
    WidgetRef->SetVisibility(ESlateVisibility::Hidden);
    WidgetRef->endSound();
}
