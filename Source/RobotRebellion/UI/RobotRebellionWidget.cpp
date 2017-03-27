// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotRebellionWidget.h"

#include "ActiveSound.h"
#include "Character/PlayableCharacter.h"


void URobotRebellionWidget::initializeOwner()
{
    APlayerController* controller = GetOwningPlayer();
    m_owner = Cast<APlayableCharacter>(controller->GetCharacter());
}

void URobotRebellionWidget::startSound()
{
    // Begin sound
    UGameplayStatics::SpawnSoundAttached(m_widgetBeginSound, m_owner->GetRootComponent());

    // Background Loop
    m_loopAudioComp = UGameplayStatics::SpawnSoundAttached(m_widgetLoopSound, m_owner->GetRootComponent());

    if(m_stopAmbiantSound)
    {
        if(GEngine)
        {
            const TArray<FActiveSound*> sounds = GEngine->GetActiveAudioDevice()->GetActiveSounds();
            for(auto sound : sounds)
            {
                UAudioComponent *audioComp = UAudioComponent::GetAudioComponentFromID(sound->GetAudioComponentID());
                if(audioComp)
                {
                    if(audioComp->GetAudioComponentID() != m_loopAudioComp->GetAudioComponentID())
                    {
                        audioComp->SetVolumeMultiplier(0.f);
                    }
                }
            }
        }
    }
}

void URobotRebellionWidget::endSound()
{
    // Stop loop
    if(m_loopAudioComp->IsPlaying())
    {
        m_loopAudioComp->Stop();
    }

    // Closing sound
    UGameplayStatics::SpawnSoundAttached(m_widgetCloseSound, m_owner->GetRootComponent());

    if(m_stopAmbiantSound)
    {
        if(GEngine)
        {
            const TArray<FActiveSound*> sounds = GEngine->GetActiveAudioDevice()->GetActiveSounds();
            for(auto sound : sounds)
            {
                UAudioComponent *audioComp = UAudioComponent::GetAudioComponentFromID(sound->GetAudioComponentID());
                if(audioComp)
                {
                    if(audioComp->GetAudioComponentID() != m_loopAudioComp->GetAudioComponentID())
                    {
                        audioComp->SetVolumeMultiplier(1.f);
                    }
                }
            }
        }
    }
}