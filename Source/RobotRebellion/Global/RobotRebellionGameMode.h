// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "GameInstaller.h"
#include "RobotRebellionGameMode.generated.h"


enum e_currentGameMode
{
	
};

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

UCLASS(minimalapi)
class ARobotRebellionGameMode : public AGameModeBase
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alteration Default")
        TSubclassOf<class UAlterationBase> m_invisibleDefault;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alteration Default")
        TSubclassOf<class UAlterationBase> m_stunDefault;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alteration Default")
        TSubclassOf<class UAlterationBase> m_shieldDefault;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug Display")
        bool m_showVolumeBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Display")
        bool m_showVolumeConnection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Display")
        bool m_showEnnemyDetectionSphere;


public:
	ARobotRebellionGameMode();

    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;

	void setBossGameMode();

	void setBossDead();

    void setStartGameMode();
};



