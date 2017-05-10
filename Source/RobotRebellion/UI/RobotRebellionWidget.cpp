// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotRebellionWidget.h"

#include "ActiveSound.h"
#include "Character/PlayableCharacter.h"

void URobotRebellionWidget::startSound()
{
    // Begin sound
    playSound(m_widgetBeginSound);

    // Background Loop
    playSound(m_widgetLoopSound);

    if(m_stopAmbiantSound && m_loopAudioComp)
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
    if(m_loopAudioComp && m_loopAudioComp->IsPlaying())
    {
        m_loopAudioComp->Stop();
    }

    // Closing sound
    playSound(m_widgetCloseSound);

    if(m_loopAudioComp && m_stopAmbiantSound)
    {
        if(GEngine)
        {
            const TArray<FActiveSound*> sounds = GEngine->GetActiveAudioDevice()->GetActiveSounds();
            for(auto sound : sounds)
            {
                UAudioComponent *audioComp = UAudioComponent::GetAudioComponentFromID(sound->GetAudioComponentID());
                if(audioComp && m_loopAudioComp)
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

void URobotRebellionWidget::playSound(USoundCue * sound)
{
    if(sound)
    {
        auto owner = GetOwningPlayer();
        if(owner)
        {
            auto charac = owner->GetCharacter();
            if(charac)
            {
                UGameplayStatics::SpawnSoundAttached(sound, charac->GetRootComponent());
            }
        }
    }
}
