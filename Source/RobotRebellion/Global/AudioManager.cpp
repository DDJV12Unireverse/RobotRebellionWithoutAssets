#include "RobotRebellion.h"
#include "AudioManager.h"
#include "ActiveSound.h"


void AudioManager::muteAllBackgroundSoundsWithException(UAudioComponent* soundToNotMute)
{
        if(GEngine)
        {
            const TArray<FActiveSound*> sounds = GEngine->GetActiveAudioDevice()->GetActiveSounds();
            for(auto sound : sounds)
            {
                UAudioComponent *audioComp = UAudioComponent::GetAudioComponentFromID(sound->GetAudioComponentID());
                if(audioComp)
                {
                    if(audioComp->GetAudioComponentID() != soundToNotMute->GetAudioComponentID())
                    {
                        audioComp->SetVolumeMultiplier(0.f);
                    }
                }
            }
        }
}

void AudioManager::playMenuSound()
{

}
void AudioManager::playBossSound()
{

}
void AudioManager::playAmbientSound()
{

}

void AudioManager::playCombatSound()
{

}


void AudioManager::playWinSound()
{

}

void AudioManager::playLoseSound()
{

}

void AudioManager::setGlobalVolume(float volume)
{

}
