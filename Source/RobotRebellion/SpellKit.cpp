// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "SpellKit.h"
#include "Spell.h" 
#include "UtilitaryFunctionLibrary.h"

// Sets default values for this component's properties
USpellKit::USpellKit()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void USpellKit::BeginPlay()
{
    Super::BeginPlay();
    // ...

    for(int i = 0; i < m_spellsClass.Num(); ++i)
    {
        USpell* tempSpell;

        UUtilitaryFunctionLibrary::createObjectFromDefault<USpell>(
            &tempSpell,
            m_spellsClass[i],
            this,
            TEXT("Spell")
            );

        if(tempSpell)
        {
            m_spells.Emplace(tempSpell);
        }
    }
}


// Called every frame
void USpellKit::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void USpellKit::cast(int32 index)
{
    if(index < m_spells.Num())
    {
        m_spells[index]->cast();
    }
}