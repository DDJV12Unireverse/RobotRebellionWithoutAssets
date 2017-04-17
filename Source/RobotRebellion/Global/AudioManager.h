#pragma once

#include "Tool/IsSingleton.h"


class ROBOTREBELLION_API AudioManager : private IsSingleton<AudioManager>
{
    GENERATED_USING_FROM_IsSingleton(AudioManager)

private:
	TArray<UAudioComponent *> m_backgroundSounds;
	void stopBackgroundMusicWithException(UAudioComponent* soundToNotMute);

public:
    void muteAllBackgroundSoundsWithException(UAudioComponent* soundToNotMute);

	void configureBackgroundMusic(UAudioComponent * audioComponent)
	{
		m_backgroundSounds.Add(audioComponent);
	}

	void playBackgroundMusic(UAudioComponent * audioComponent);

    //void setGlobalVolume(float volume);

    //private:
    //float m_globalVolume = 1.0;

};