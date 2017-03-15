// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LobbyUIWidget.h"
#include "GameMenu.h"

AGameMenu::AGameMenu()
{
    AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
    AudioComp->bAutoActivate = true;
    AudioComp->Activate();
    AudioComp->Play();
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

    AudioComp->SetSound(MenuLoop);
    AudioComp->Play();
}
