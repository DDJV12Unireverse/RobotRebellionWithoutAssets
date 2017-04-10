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
    HUDCharacterImpl->SetVisibility(ESlateVisibility::Hidden);

    LobbyImpl = CreateCustomWidget<ULobbyUIWidget>(Cast<ULobbyUIWidget>(LobbyWidget->GetDefaultObject()));
    LobbyImpl->initialiseOnliSubsystem();
    LobbyImpl->SetVisibility(ESlateVisibility::Hidden);

    ReviveTimerWidgetImpl = CreateCustomWidget<UReviveTimerWidget>(ReviveWidget.GetDefaultObject());
    ReviveTimerWidgetImpl->SetVisibility(ESlateVisibility::Hidden);

    ClassSelectionWidgetImpl = CreateCustomWidget<URobotRebellionWidget>(ClassSelectionWidget.GetDefaultObject());
    DisplayWidget(ClassSelectionWidgetImpl);
}

void AGameMenu::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    APlayableCharacter* player = Cast<APlayableCharacter>(GetOwningPlayerController()->GetCharacter());
    if(player)
    {
        if(player->isReviving())
        {
            ReviveTimerWidgetImpl->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ReviveTimerWidgetImpl->SetVisibility(ESlateVisibility::Hidden);
        }
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
