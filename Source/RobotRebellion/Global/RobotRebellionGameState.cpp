// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotRebellionGameState.h"
#include "Character/PlayableCharacter.h"
#include "RobotRebellionGameMode.h"
#include "AudioManager.h"
#include "EntityDataSingleton.h"





ARobotRebellionGameState::ARobotRebellionGameState()
{
//     m_gameMode = ECurrentGameMode::INTRO;
//     m_previousGameMode = ECurrentGameMode::NONE;
//     m_bossIsDead = false;
//     m_gameIsStarted = false;
//     PrimaryActorTick.bCanEverTick = true;
}

// void ARobotRebellionGameState::BeginPlay()
// {
//     Super::BeginPlay();
//     setupAudioComponents();
// }

// void ARobotRebellionGameState::Tick(float DeltaTime)
// {
//     Super::Tick(DeltaTime);
// 
//     EntityDataSingleton& data = EntityDataSingleton::getInstance();
// 
//     bool playerInCombat = false;
//     UWorld* w = this->GetWorld();
//     ARobotRebellionGameMode* gameMode = Cast<ARobotRebellionGameMode>(w->GetAuthGameMode());
//     //auto data = gameMode->data;data.m_playableCharacterArray.Num()
//     for(int i = 0; i < data.m_playableCharacterArray.Num(); i++)
//     {
//         APlayableCharacter* playableCharacter = data.m_playableCharacterArray[i];
//         if(playableCharacter->m_isInCombat)
//         {
//             playerInCombat = true;
//         }
//     }
//     if(playerInCombat && m_gameMode != ECurrentGameMode::BOSS)
//     {
//         m_gameMode = ECurrentGameMode::COMBAT;
//     }
//     else if(m_gameIsStarted && m_gameMode != ECurrentGameMode::BOSS)
//     {
//         m_gameMode = ECurrentGameMode::AMBIENT;
//     }
//     if(m_bossIsDead)
//     {
//         m_gameMode = ECurrentGameMode::WIN;
//     }
// 
//     if(m_gameMode != m_previousGameMode)
//     {
//         AudioManager& audioMan = AudioManager::getInstance();
//         switch(m_gameMode)
//         {
//             case ECurrentGameMode::INTRO:
//                 audioMan.playBackgroundMusic(m_introAudioComp);
//                 break;
//             case ECurrentGameMode::NONE:
//             case ECurrentGameMode::AMBIENT:
//                 audioMan.playBackgroundMusic(m_ambientAudioComp);
//                 break;
//             case ECurrentGameMode::COMBAT:
//                 audioMan.playBackgroundMusic(m_combatAudioComp);
//                 break;
//             case ECurrentGameMode::BOSS:
//                 audioMan.playBackgroundMusic(m_bossAudioComp);
//                 break;
//             case ECurrentGameMode::WIN:
//                 audioMan.playBackgroundMusic(m_winAudioComp);
//                 break;
//             case ECurrentGameMode::LOSE:
//                 audioMan.playBackgroundMusic(m_loseAudioComp);
//                 break;
//         }
//     }
// 
//     m_previousGameMode = m_gameMode;
// }


// void ARobotRebellionGameState::setBossGameMode()
// {
//     m_gameMode = ECurrentGameMode::BOSS;
// //     if (Role>=ROLE_Authority)
// //     {
// //         multiSetBossGameMode();
// //     }
// }

// void ARobotRebellionGameState::multiSetBossGameMode_Implementation()
// {
//     m_gameMode = ECurrentGameMode::BOSS;
// }
// 
// void ARobotRebellionGameState::serverSetBossGameMode()
// {
// 
// }

// void ARobotRebellionGameState::setBossDead()
// {
//     m_bossIsDead = true;
// //     if(Role >= ROLE_Authority)
// //     {
// //         multiSetBossDead();
// //     }
// }

// void ARobotRebellionGameState::multiSetBossDead_Implementation()
// {
//     m_bossIsDead = true;
// }
// 
// void ARobotRebellionGameState::serverSetBossDead()
// {
// 
// }

// void ARobotRebellionGameState::setStartGameMode()
// {
//     m_gameIsStarted = true;
// //     if(Role >= ROLE_Authority)
// //     {
// //         multiSetStartGameMode();
// //     }
// }

// void ARobotRebellionGameState::multiSetStartGameMode_Implementation()
// {
//     m_gameIsStarted = true;
// }


// void ARobotRebellionGameState::serverSetStartGameMode_Implementation()
// {
//     setStartGameMode();
// }

// bool ARobotRebellionGameState::serverSetStartGameMode_Validate()
// {
//     return true;
// }

// void ARobotRebellionGameState::setupAudioComponents()
// {
// //     if (Role<ROLE_Authority)
// //     {
// //         serverSetupAudioComponents();
// //         return;
// //     }
// 
//     if(m_introSounds && !m_introAudioComp)
//     {
//         m_introAudioComp = NewObject<UAudioComponent>(this);
//         m_introAudioComp->SetSound(m_introSounds);
//     }
//     if(m_ambientSounds && !m_ambientAudioComp)
//     {
//         m_ambientAudioComp = NewObject<UAudioComponent>(this);
//         m_ambientAudioComp->SetSound(m_ambientSounds);
//     }
//     if(m_combatSounds && !m_combatAudioComp)
//     {
//         m_combatAudioComp = NewObject<UAudioComponent>(this);
//         m_combatAudioComp->SetSound(m_combatSounds);
//     }
//     if(m_bossSounds && !m_bossAudioComp)
//     {
//         m_bossAudioComp = NewObject<UAudioComponent>(this);
//         m_bossAudioComp->SetSound(m_bossSounds);
//     }
//     if(m_winSounds && !m_winAudioComp)
//     {
//         m_winAudioComp = NewObject<UAudioComponent>(this);
//         m_winAudioComp->SetSound(m_winSounds);
//     }
//     if(m_loseSounds && !m_loseAudioComp)
//     {
//         m_loseAudioComp = NewObject<UAudioComponent>(this);
//         m_loseAudioComp->SetSound(m_loseSounds);
//     }
// 
// //     if (Role>=ROLE_Authority)
// //     {
// //         multiSetupAudioComponents();
// //     }
// }

// void ARobotRebellionGameState::serverSetupAudioComponents_Implementation()
// {
//     setupAudioComponents();
// }
// bool ARobotRebellionGameState::serverSetupAudioComponents_Validate()
// {
//     return true;
// }

// void ARobotRebellionGameState::multiSetupAudioComponents_Implementation()
// {
//     if(m_introSounds && !m_introAudioComp)
//     {
//         m_introAudioComp = NewObject<UAudioComponent>(this);
//         m_introAudioComp->SetSound(m_introSounds);
//     }
//     if(m_ambientSounds && !m_ambientAudioComp)
//     {
//         m_ambientAudioComp = NewObject<UAudioComponent>(this);
//         m_ambientAudioComp->SetSound(m_ambientSounds);
//     }
//     if(m_combatSounds && !m_combatAudioComp)
//     {
//         m_combatAudioComp = NewObject<UAudioComponent>(this);
//         m_combatAudioComp->SetSound(m_combatSounds);
//     }
//     if(m_bossSounds && !m_bossAudioComp)
//     {
//         m_bossAudioComp = NewObject<UAudioComponent>(this);
//         m_bossAudioComp->SetSound(m_bossSounds);
//     }
//     if(m_winSounds && !m_winAudioComp)
//     {
//         m_winAudioComp = NewObject<UAudioComponent>(this);
//         m_winAudioComp->SetSound(m_winSounds);
//     }
//     if(m_loseSounds && !m_loseAudioComp)
//     {
//         m_loseAudioComp = NewObject<UAudioComponent>(this);
//         m_loseAudioComp->SetSound(m_loseSounds);
//     }
// }
