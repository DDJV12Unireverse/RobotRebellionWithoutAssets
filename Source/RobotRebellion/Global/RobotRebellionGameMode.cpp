// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.


#include "RobotRebellion.h"
#include "AudioManager.h"
#include "RobotRebellionGameMode.h"
#include "Character/RobotRebellionCharacter.h"

#include "GameInstaller.h"
#include "Gameplay/Alteration/StunAlteration.h"
#include "Gameplay/Alteration/InvisibilityAlteration.h"

#include "EntityDataSingleton.h"
#include "IA/Navigation/NavigationVolumeGraph.h"
#include "Tool/UtilitaryMacros.h"

ARobotRebellionGameMode::ARobotRebellionGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> defaultPawn(TEXT("/Game/ThirdPersonCPP/Blueprints/DefaultSpawn_BP"));
    DefaultPawnClass = defaultPawn.Class;
    bUseSeamlessTravel = true;

    PrimaryActorTick.bCanEverTick = true;

}

void ARobotRebellionGameMode::BeginPlay()
{
    if(m_combatSound && !m_gameAudioComp)
    {
        //m_gameAudioComp = UGameplayStatics::SpawnSoundAttached(m_combatSound, UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        m_gameAudioComp = NewObject<UAudioComponent>(this);
        m_gameAudioComp->SetSound(m_combatSound);
    }

    Super::BeginPlay();

    GameAlterationInstaller& installer = GameAlterationInstaller::getInstance();
    installer.installAlteration<UStunAlteration>(&m_stunDefault);
    installer.installAlteration<UInvisibilityAlteration>(&m_invisibleDefault);
    installer.installAlteration<UShieldAlteration>(&m_shieldDefault);

    EntityDataSingleton& datas = EntityDataSingleton::getInstance();
    datas.m_showVolumeBox = this->m_showVolumeBox;
    datas.m_showEnnemyDetectionSphere = this->m_showEnnemyDetectionSphere;

    NavigationVolumeGraph& navGraph = NavigationVolumeGraph::getInstance();
    navGraph.m_showConnection = this->m_showVolumeConnection;

}

void ARobotRebellionGameMode::Tick(float deltaTime)
{
    EntityDataSingleton& data = EntityDataSingleton::getInstance();
    data.update(this->GetWorld());
    for(int i = 0 ; i < data.m_playableCharacterArray.Num() ; i++)
    {
        APlayableCharacter* playableCharacter = data.m_playableCharacterArray[i];
        // PLAY COMBAT SOUND
        if(playableCharacter->m_isInCombat)
        {
            //TODO
            //AudioManager& audioMan = AudioManager::getInstance();
            //audioMan.playCombatSound();


            // Background Loop
            if(m_gameAudioComp && !m_gameAudioComp->IsPlaying())
            {
                m_gameAudioComp->Play();
            }

            //// TEST AUDIO FOR NOW UNTIL MANAGER IS DONE
            if(GEngine && m_gameAudioComp)
            {
                const TArray<FActiveSound*> sounds = GEngine->GetActiveAudioDevice()->GetActiveSounds();
                for(auto sound : sounds)
                {
                    UAudioComponent *audioComp = UAudioComponent::GetAudioComponentFromID(sound->GetAudioComponentID());
                    if(audioComp)
                    {
                        if(audioComp->GetAudioComponentID() != m_gameAudioComp->GetAudioComponentID())
                        {
                            audioComp->Stop();
                        }
                    }
                }
            }
            //PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "GameMode IS IN Combat Player: " + FString::FromInt(playableCharacter->m_isInCombat));
        }
        else
        {
            if(m_gameAudioComp && m_combatSound)
            {
                // Stop loop
                if(m_gameAudioComp->IsPlaying())
                {
                    //m_gameAudioComp->SetVolumeMultiplier(0.f);
                    m_gameAudioComp->Stop();
                }
            }
        }

    }
}
