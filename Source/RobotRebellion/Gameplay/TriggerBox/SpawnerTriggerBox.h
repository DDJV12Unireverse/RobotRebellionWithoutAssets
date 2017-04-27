// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "SpawnerTriggerBox.generated.h"

/**
 *
 */
UCLASS()
class ROBOTREBELLION_API ASpawnerTriggerBox : public ATriggerBox
{
    GENERATED_BODY()


public:
    /************************************************************************/
    /* UPROPERTY                                                            */
    /************************************************************************/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search")
        float m_scope;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CONDITION")
        uint8 m_destroyWhenPassed : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CONDITION")
        uint8 m_autoActivateCombat : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CONDITION")
        uint8 m_activeOnlyWhenKingHere : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CONDITION")
        uint8 m_random : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CONDITION")
        uint8 m_noSpawnWhenPopulated : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CONDITION")
        uint8 m_relativePosition : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CONDITION")
        uint8 m_effectOnSpawn : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CONDITION")
        uint8 m_comeFromUpper : 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Population")
        float m_upperOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Population")
        TArray<FTransform> m_populationTransform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Population")
        TArray<TSubclassOf<class ANonPlayableCharacter>> m_populationToSpawn;


    /************************************************************************/
    /* PROPERTY                                                             */
    /************************************************************************/
    TArray<class ARobotRebellionCharacter*> m_characterOnBox;
    TArray<class APawn*> m_spawned;

    float m_maxDist; 
    
    FActorSpawnParameters m_spawnParams;


public:
    /************************************************************************/
    /* CONSTRUCTOR                                                          */
    /************************************************************************/
    ASpawnerTriggerBox();



    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/
    virtual void BeginPlay() override;


private:
    void correctDestruction();

    void killItself();

    void internalSpawn();

    void internalSpawnCharacterAtIndex(int32 index, UWorld* world);

    void setNearTarget(class ANonPlayableCharacter* spawned);

    void checkCharactersOnBox();


public:
    /************************************************************************/
    /* UFUNCTION                                                            */
    /************************************************************************/
    UFUNCTION()
        void onHit(UPrimitiveComponent* var1, AActor* enteredActor, UPrimitiveComponent* var3, int32 var4, bool var5, const FHitResult& var6);

    UFUNCTION()
        void spawnEnnemies();


private:
    UFUNCTION(Reliable, Server, WithValidation)
        void serverKills();

    UFUNCTION(Reliable, NetMulticast)
        void multiKills();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverSpawnEnnemies();
};
