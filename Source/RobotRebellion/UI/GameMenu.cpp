// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LobbyUIWidget.h"
#include "GameMenu.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "AudioDevice.h"
#include "ActiveSound.h"

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
    if (LobbyImpl->Visibility == ESlateVisibility::Hidden && AudioComp->IsPlaying())
    {
        if (GEngine)
        {
            auto sounds = GEngine->GetActiveAudioDevice()->GetActiveSounds();
            //GEngine->GetActiveAudioDevice()->PauseActiveSound();
            // GEngine->GetActiveAudioDevice()->
            for (auto sound : sounds)
            {
                UAudioComponent *audioComp = UAudioComponent::GetAudioComponentFromID(sound->GetAudioComponentID());
                if (audioComp->GetAudioComponentID() != AudioComp->GetAudioComponentID())
                    //audioComp->Stop();
                    GEngine->GetActiveAudioDevice()->PauseActiveSound(audioComp->GetAudioComponentID(), false);
            }
        }
        AudioComp->Stop();
    }
   
}

void AGameMenu::DisplayWidget(UUserWidget* WidgetRef)
{
    WidgetRef->SetVisibility(ESlateVisibility::Visible);
    if (GEngine)
    {
        auto sounds = GEngine->GetActiveAudioDevice()->GetActiveSounds();
        //GEngine->GetActiveAudioDevice()->PauseActiveSound();
       // GEngine->GetActiveAudioDevice()->
        for (auto sound : sounds)
        {
            UAudioComponent *audioComp = UAudioComponent::GetAudioComponentFromID(sound->GetAudioComponentID());
            if (audioComp->GetAudioComponentID() !=AudioComp->GetAudioComponentID())
            //audioComp->Stop();
            GEngine->GetActiveAudioDevice()->PauseActiveSound(audioComp->GetAudioComponentID(),true);
        }
    }
    AudioComp->SetSound(MenuLoop);
    AudioComp->Play();

}

void AGameMenu::HideWidget(UUserWidget* WidgetRef)
{
    WidgetRef->SetVisibility(ESlateVisibility::Hidden);
}
