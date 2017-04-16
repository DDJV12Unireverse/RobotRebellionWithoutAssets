#pragma once

#include "Tool/IsSingleton.h"


class ROBOTREBELLION_API AudioManager : private IsSingleton<AudioManager>
{
    GENERATED_USING_FROM_IsSingleton(AudioManager)



public:
    void muteAllBackgroundSoundsWithException(UAudioComponent* soundToNotMute);

    void playMenuSound();

    void playBossSound();

    void playAmbientSound();

    void playCombatSound();

    void playWinSound();

    void playLoseSound();

    void setGlobalVolume(float volume);

    private:
    float m_globalVolume = 1.0;

};