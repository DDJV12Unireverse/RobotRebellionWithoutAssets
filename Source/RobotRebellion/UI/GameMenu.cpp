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
    AudioComp->bAutoDestroy = false; 
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
            const TArray<FActiveSound*> sounds = GEngine->GetActiveAudioDevice()->GetActiveSounds();
            for (auto sound : sounds)
            {
                UAudioComponent *audioComp = UAudioComponent::GetAudioComponentFromID(sound->GetAudioComponentID());
                if(audioComp)
                {
                    if(audioComp->GetAudioComponentID() != AudioComp->GetAudioComponentID())
                    {
                        audioComp->SetVolumeMultiplier(1.f);
                    }
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
        const TArray<FActiveSound*> sounds = GEngine->GetActiveAudioDevice()->GetActiveSounds();
        for (auto sound : sounds)
        {
            UAudioComponent *audioComp = UAudioComponent::GetAudioComponentFromID(sound->GetAudioComponentID());
            if(audioComp)
            {
                if (audioComp->GetAudioComponentID() !=AudioComp->GetAudioComponentID())
                {
                    audioComp->SetVolumeMultiplier(0.f);
                }
            }
        }
    }
    AudioComp->SetSound(MenuLoop);
    AudioComp->Play();
}

void AGameMenu::HideWidget(UUserWidget* WidgetRef)
{
    WidgetRef->SetVisibility(ESlateVisibility::Hidden);
}
