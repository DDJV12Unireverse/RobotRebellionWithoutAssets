// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ClassType.h"
#include "RobotRobellionSpawnerClass.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTREBELLION_API URobotRobellionSpawnerClass : public UActorComponent
{
	GENERATED_BODY()


public:
    /************************************************************************/
    /* UPROPERTY                                                            */
    /************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSubclassOf<class APlayableCharacter> m_assassinActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSubclassOf<APlayableCharacter> m_healerActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSubclassOf<APlayableCharacter> m_soldierActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSubclassOf<APlayableCharacter> m_wizardActor;



    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

	// Sets default values for this component's properties
	URobotRobellionSpawnerClass();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
    /************************************************************************/
    /* UFUNCTION                                                            */
    /************************************************************************/
    UFUNCTION(Reliable, Server, WithValidation)
        void serverSpawnAndReplace(APlayableCharacter* owner, EClassType typeToChange);


    //Replace the player with a new character. Specify the player and the new type of character.
    UFUNCTION(BlueprintCallable, Category = "")
        void spawnAndReplace(APlayableCharacter* owner, EClassType typeToChange);
};
