// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LobbyUIWidget.h"
#include "GameMenu.h"
#include "Character/PlayableCharacter.h"

AGameMenu::AGameMenu()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGameMenu::firstCallHUD_Implementation()
{
    // does nothing
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
    ClassSelectionWidgetImpl->SetVisibility(ESlateVisibility::Hidden);

    OptionsWidgetImpl = CreateCustomWidget<UOptionsMenuWidget>(OptionsWidget.GetDefaultObject());
    OptionsWidgetImpl->SetVisibility(ESlateVisibility::Hidden);

    TopWidgetImpl = CreateCustomWidget<UTopWidget>(TopWidget.GetDefaultObject());
    TopWidgetImpl->SetVisibility(ESlateVisibility::Hidden);

    firstCallHUD();
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
