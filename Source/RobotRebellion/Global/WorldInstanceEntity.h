// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WorldInstanceEntity.generated.h"

UCLASS()
class ROBOTREBELLION_API AWorldInstanceEntity : public AActor
{
	GENERATED_BODY()
	
public:

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


	// Sets default values for this actor's properties
	AWorldInstanceEntity();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
};
