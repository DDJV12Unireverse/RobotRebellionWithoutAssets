// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WorldInstanceEntity.generated.h"

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
class ROBOTREBELLION_API AWorldInstanceEntity : public AActor
{
    GENERATED_BODY()

private:
    UPROPERTY(VisibleDefaultsOnly)
        ECurrentGameMode m_gameMode;


    ECurrentGameMode m_previousGameMode;

    bool m_bossIsDead;
    bool m_gameIsStarted;
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug Display")
        bool m_showVolumeBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Display")
        bool m_showVolumeConnection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Display")
        bool m_showEnnemyDetectionSphere;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alteration Default")
        TSubclassOf<class UAlterationBase> m_invisibleDefault;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alteration Default")
        TSubclassOf<class UAlterationBase> m_stunDefault;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alteration Default")
        TSubclassOf<class UAlterationBase> m_shieldDefault;


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


// Sets default values for this actor's properties
    AWorldInstanceEntity();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;

    void setBossGameMode();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverSetBossGameMode();

    UFUNCTION(Reliable, NetMulticast)
        void multiSetBossGameMode();

    void setBossDead();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverSetBossDead();
    UFUNCTION(Reliable, NetMulticast)
        void multiSetBossDead();

    void setStartGameMode();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverSetStartGameMode();
    UFUNCTION(Reliable, NetMulticast)
        void multiSetStartGameMode();

    void setupAudioComponents();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverSetupAudioComponents();
    UFUNCTION(Reliable, NetMulticast)
        void multiSetupAudioComponents();

private:
    void internalSetupAudioComponents();
};
