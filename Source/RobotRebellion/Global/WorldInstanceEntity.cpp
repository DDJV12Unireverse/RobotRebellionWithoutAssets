// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "WorldInstanceEntity.h"


// Sets default values
AWorldInstanceEntity::AWorldInstanceEntity()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    m_gameMode = ECurrentGameMode::INTRO;
    m_previousGameMode = ECurrentGameMode::NONE;
    m_bossIsDead = false;
    m_gameIsStarted = false;
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldInstanceEntity::BeginPlay()
{
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

    setupAudioComponents();
}

// Called every frame
void AWorldInstanceEntity::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    EntityDataSingleton::getInstance().update(this->GetWorld());

    EntityDataSingleton& data = EntityDataSingleton::getInstance();

    bool playerInCombat = false;
    //UWorld* w = this->GetWorld();
    //ARobotRebellionGameMode* gameMode = Cast<ARobotRebellionGameMode>(w->GetAuthGameMode());
    //auto data = gameMode->data;data.m_playableCharacterArray.Num()
    for(int i = 0; i < data.m_playableCharacterArray.Num(); i++)
    {
        APlayableCharacter* playableCharacter = data.m_playableCharacterArray[i];
        if(playableCharacter->m_isInCombat)
        {
            playerInCombat = true;
        }
    }
    if(playerInCombat && m_gameMode != ECurrentGameMode::BOSS)
    {
        m_gameMode = ECurrentGameMode::COMBAT;
    }
    else if(m_gameIsStarted && m_gameMode != ECurrentGameMode::BOSS)
    {
        m_gameMode = ECurrentGameMode::AMBIENT;
    }
    if(m_bossIsDead)
    {
        m_gameMode = ECurrentGameMode::WIN;
    }

    if(m_gameMode != m_previousGameMode)
    {
        AudioManager& audioMan = AudioManager::getInstance();
        switch(m_gameMode)
        {
            case ECurrentGameMode::INTRO:
                audioMan.playBackgroundMusic(m_introAudioComp);
                break;
            case ECurrentGameMode::NONE:
            case ECurrentGameMode::AMBIENT:
                audioMan.playBackgroundMusic(m_ambientAudioComp);
                break;
            case ECurrentGameMode::COMBAT:
                audioMan.playBackgroundMusic(m_combatAudioComp);
                break;
            case ECurrentGameMode::BOSS:
                audioMan.playBackgroundMusic(m_bossAudioComp);
                break;
            case ECurrentGameMode::WIN:
                audioMan.playBackgroundMusic(m_winAudioComp);
                break;
            case ECurrentGameMode::LOSE:
                audioMan.playBackgroundMusic(m_loseAudioComp);
                break;
        }
    }

    m_previousGameMode = m_gameMode;

}

void AWorldInstanceEntity::setBossGameMode()
{
    if (Role<ROLE_Authority)
    {
        serverSetBossGameMode();
    }
    m_gameMode = ECurrentGameMode::BOSS;
    if (Role>=ROLE_Authority)
    {
        multiSetBossGameMode();
    }
}

void AWorldInstanceEntity::serverSetBossGameMode_Implementation()
{
    serverSetBossGameMode();
}

bool AWorldInstanceEntity::serverSetBossGameMode_Validate()
{
    return true;
}

void AWorldInstanceEntity::multiSetBossGameMode_Implementation()
{
    m_gameMode = ECurrentGameMode::BOSS;

}

// void AWorldInstanceEntity::Tick(float DeltaTime)

void AWorldInstanceEntity::setBossDead()
{
    if(Role < ROLE_Authority)
    {
        serverSetBossDead();
    }
    m_bossIsDead = true;
    if(Role >= ROLE_Authority)
    {
        multiSetBossDead();
    }
}

void AWorldInstanceEntity::serverSetBossDead_Implementation()
{
    setBossDead();
}

bool AWorldInstanceEntity::serverSetBossDead_Validate()
{
    return true;
}

void AWorldInstanceEntity::multiSetBossDead_Implementation()
{
    m_bossIsDead = true;
}

void AWorldInstanceEntity::setStartGameMode()
{
    if (Role<ROLE_Authority)
    {
        serverSetStartGameMode();
    }
    m_gameIsStarted = true;
    if(Role >= ROLE_Authority)
    {
        multiSetStartGameMode();
    }
}

void AWorldInstanceEntity::serverSetStartGameMode_Implementation()
{
    setStartGameMode();
}

bool AWorldInstanceEntity::serverSetStartGameMode_Validate()
{
    return true;
}

void AWorldInstanceEntity::multiSetStartGameMode_Implementation()
{
    m_gameIsStarted = true;
}

void AWorldInstanceEntity::setupAudioComponents()
{
    if(Role < ROLE_Authority)
    {
        serverSetupAudioComponents();
        //return;
    }

    if(m_introSounds && !m_introAudioComp)
    {
        m_introAudioComp = NewObject<UAudioComponent>(this);
        m_introAudioComp->SetSound(m_introSounds);
    }
    if(m_ambientSounds && !m_ambientAudioComp)
    {
        m_ambientAudioComp = NewObject<UAudioComponent>(this);
        m_ambientAudioComp->SetSound(m_ambientSounds);
    }
    if(m_combatSounds && !m_combatAudioComp)
    {
        m_combatAudioComp = NewObject<UAudioComponent>(this);
        m_combatAudioComp->SetSound(m_combatSounds);
    }
    if(m_bossSounds && !m_bossAudioComp)
    {
        m_bossAudioComp = NewObject<UAudioComponent>(this);
        m_bossAudioComp->SetSound(m_bossSounds);
    }
    if(m_winSounds && !m_winAudioComp)
    {
        m_winAudioComp = NewObject<UAudioComponent>(this);
        m_winAudioComp->SetSound(m_winSounds);
    }
    if(m_loseSounds && !m_loseAudioComp)
    {
        m_loseAudioComp = NewObject<UAudioComponent>(this);
        m_loseAudioComp->SetSound(m_loseSounds);
    }

    if(Role >= ROLE_Authority)
    {
        multiSetupAudioComponents();
    }
}

void AWorldInstanceEntity::serverSetupAudioComponents_Implementation()
{
    setupAudioComponents();
}

bool AWorldInstanceEntity::serverSetupAudioComponents_Validate()
{
    return true;
}

void AWorldInstanceEntity::multiSetupAudioComponents_Implementation()
{
    if(m_introSounds && !m_introAudioComp)
    {
        m_introAudioComp = NewObject<UAudioComponent>(this);
        m_introAudioComp->SetSound(m_introSounds);
    }
    if(m_ambientSounds && !m_ambientAudioComp)
    {
        m_ambientAudioComp = NewObject<UAudioComponent>(this);
        m_ambientAudioComp->SetSound(m_ambientSounds);
    }
    if(m_combatSounds && !m_combatAudioComp)
    {
        m_combatAudioComp = NewObject<UAudioComponent>(this);
        m_combatAudioComp->SetSound(m_combatSounds);
    }
    if(m_bossSounds && !m_bossAudioComp)
    {
        m_bossAudioComp = NewObject<UAudioComponent>(this);
        m_bossAudioComp->SetSound(m_bossSounds);
    }
    if(m_winSounds && !m_winAudioComp)
    {
        m_winAudioComp = NewObject<UAudioComponent>(this);
        m_winAudioComp->SetSound(m_winSounds);
    }
    if(m_loseSounds && !m_loseAudioComp)
    {
        m_loseAudioComp = NewObject<UAudioComponent>(this);
        m_loseAudioComp->SetSound(m_loseSounds);
    }
}
