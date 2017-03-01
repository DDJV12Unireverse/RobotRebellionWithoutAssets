// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SpellKit.generated.h"

/*
 * Handle a skill set for a playable Character or maybe the boss
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTREBELLION_API USpellKit : public UActorComponent
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellKit)
        TArray<TSubclassOf<class USpell>> m_spellsClass; // forward decl

private:
    TArray<USpell *> m_spells;

public:	
	// Sets default values for this component's properties
	USpellKit();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    // cast the spell matching the index if it exists
    void cast(int32 index);

    //Initialize spells list
    void initializeSpells();
	
};
