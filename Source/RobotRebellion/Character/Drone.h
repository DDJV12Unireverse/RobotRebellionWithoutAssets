// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NonPlayableCharacter.h"
#include "Drone.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ADrone : public ANonPlayableCharacter
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
        float m_debugAutoDropTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge")
        FVector m_bombAccroch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge")
        TSubclassOf<class AKaboom> m_defaultKaboomBomb;


    class AKaboom* m_currentBomb;

    float m_debugTimer;


public:
    /************************************************************************/
    /* CONSTRUCTOR                                                          */
    /************************************************************************/
    ADrone();


    /************************************************************************/
    /* GENUINE METHODS                                                      */
    /************************************************************************/
    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;


    void displayScore(float scores[5]);


    /************************************************************************/
    /* UFUNCTION                                                            */
    /************************************************************************/

    /*
    Reload the drone with an object (here a kaboom bomb). Spawn the Kaboom actor and attach it to the drone. 
    The Bomb is unnactive and won't explode at collision.
    Return true if the reloading was successful, false otherwise (the bomb was not created or it has already a bomb)
    */
    UFUNCTION(BlueprintCallable, Category = "Action")
        bool reload();

    /*
    Drop the object the drone currently has (here the kaboom bomb).
    Does nothing if the drone has no attached object.
    The bomb will be activated when launched (will explode at collision).
    */
    UFUNCTION(BlueprintCallable, Category = "Action")
        void drop();

    /*
    Get the bomb radius. Warning : check if the drone is loaded before using this method.
    */
    UFUNCTION(BlueprintCallable, Category = "Action")
        float getBombRadius() const USE_NOEXCEPT;

    /*
    Get the bomb base damage. Warning : check if the drone is loaded before using this method.
    */
    UFUNCTION(BlueprintCallable, Category = "Action")
        float getBombBaseDamage() const USE_NOEXCEPT;

    UFUNCTION(BlueprintCallable, Category = "Action")
        FORCEINLINE bool isLoaded() const USE_NOEXCEPT
    {
        return m_currentBomb != nullptr;
    }

    UFUNCTION(BlueprintCallable, Category = "Debug")
        void autoDrop(float deltaTime);

};
