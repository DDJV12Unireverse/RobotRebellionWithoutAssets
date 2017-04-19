// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "RobotRebellionGameState.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECurrentGameMode : uint8
{
    NONE,
    INTRO,
    AMBIENT,
    COMBAT,
    BOSS,
    WIN,
    LOSE,
    GAME_MODE_COUNT
};

UCLASS()
class ROBOTREBELLION_API ARobotRebellionGameState : public AGameStateBase
{
	GENERATED_BODY()
private:
    

    

    UPROPERTY(VisibleDefaultsOnly)
        ECurrentGameMode m_gameMode;

    void setupAudioComponents();
    ECurrentGameMode m_previousGameMode;

    bool m_bossIsDead;
    bool m_gameIsStarted;



public:
    UPROPERTY(VisibleDefaultsOnly)
        UAudioComponent* m_introAudioComp;

    UPROPERTY(VisibleDefaultsOnly)
        UAudioComponent* m_ambientAudioComp;

    UPROPERTY(VisibleDefaultsOnly)
        UAudioComponent* m_combatAudioComp;

    UPROPERTY(VisibleDefaultsOnly)
        UAudioComponent* m_bossAudioComp;

    UPROPERTY(VisibleDefaultsOnly)
        UAudioComponent* m_winAudioComp;

    UPROPERTY(VisibleDefaultsOnly)
        UAudioComponent* m_loseAudioComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soundcues")
        USoundCue* m_introSounds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soundcues")
        USoundCue* m_ambientSounds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soundcues")
        USoundCue* m_combatSounds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soundcues")
        USoundCue* m_bossSounds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soundcues")
        USoundCue* m_winSounds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soundcues")
        USoundCue* m_loseSounds;
	
    ARobotRebellionGameState();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    void setBossGameMode();

    void setBossDead();

    void setStartGameMode();
	
};
