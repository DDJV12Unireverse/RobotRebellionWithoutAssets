// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EditorGraphVolume.generated.h"

UCLASS()
class ROBOTREBELLION_API AEditorGraphVolume : public AActor
{
	GENERATED_BODY()

private:
    /** Unique ID that defines a navigationVolume, automatically calculed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Volume)*/
        int32 m_id;
public:
    /** An array to all volume that are connected to this one */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Volume)
        TArray<AEditorGraphVolume*> m_neighbour;
    /** used to set the size of the volume, an to have a visual feedback*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Volume)
        UBoxComponent* m_box;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
        bool m_isVisibleInGame;

public:
    // Sets default values for this actor's properties
    AEditorGraphVolume();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;

    int32 getId() const
    {
        return m_id;
    }

    // Return true if the volume overlap the given point
    bool contains(const FVector& point) const;

    // Return the distance if the volume below the point
    // else return -1;
    float isBelow(const FVector& point) const;
private:
    // register the volume into the graph
    void registerNode();
	
};
