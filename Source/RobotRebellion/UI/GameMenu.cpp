// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LobbyUIWidget.h"
#include "GameMenu.h"

AGameMenu::AGameMenu()
{
    AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
    AudioComp->bAutoActivate = false;
    AudioComp->bAutoDestroy = false; ///TEST true
    AudioComp->SetupAttachment(RootComponent);
}

void AGameMenu::BeginPlay()
{
    Super::BeginPlay();

    HUDCharacterImpl = CreateCustomWidget<UCustomRobotRebellionUserWidget>(HUDCharacterWidget.GetDefaultObject());
    //HUDCharacterImpl->SetVisibility(ESlateVisibility::Hidden);

    LobbyImpl = CreateCustomWidget<ULobbyUIWidget>(Cast<ULobbyUIWidget>(LobbyWidget->GetDefaultObject()));
    LobbyImpl->initialiseOnliSubsystem();
    HideWidget(LobbyImpl);
}
void AGameMenu::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    if (LobbyImpl->Visibility==ESlateVisibility::Hidden && AudioComp->IsPlaying())
    {
        AudioComp->Stop();
    }
}

void AGameMenu::DisplayWidget(UUserWidget* WidgetRef)
{
    WidgetRef->SetVisibility(ESlateVisibility::Visible);
    AudioComp->SetSound(MenuLoop);
    AudioComp->Play();
}

void AGameMenu::HideWidget(UUserWidget* WidgetRef)
{
    WidgetRef->SetVisibility(ESlateVisibility::Hidden);
}
