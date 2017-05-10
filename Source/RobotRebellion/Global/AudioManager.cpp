#include "RobotRebellion.h"
#include "AudioManager.h"
#include "ActiveSound.h"


void AudioManager::stopBackgroundMusicWithException(UAudioComponent* soundToNotMute)
{
	if (GEngine)
	{
		const TArray<FActiveSound*> sounds = GEngine->GetActiveAudioDevice()->GetActiveSounds();
		for (auto sound : sounds)
		{
			UAudioComponent *audioComp = UAudioComponent::GetAudioComponentFromID(sound->GetAudioComponentID());
			if (audioComp)
			{
				if (audioComp->GetAudioComponentID() != soundToNotMute->GetAudioComponentID())
				{
					audioComp->Stop();
				}
			}
		}
	}
}

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

void AudioManager::playBackgroundMusic(UAudioComponent * audioComponent)
{
	if (audioComponent)
	{
		stopBackgroundMusicWithException(audioComponent);
		if(!audioComponent->IsPlaying())
		{
			audioComponent->Play();
		}
	}
}

//
//void AudioManager::setGlobalVolume(float volume)
//{
//
//}
