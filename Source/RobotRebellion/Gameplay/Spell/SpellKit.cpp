// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "SpellKit.h"
#include "Spell.h" 
#include "ThrowSpell.h"
#include "../../Tool/UtilitaryFunctionLibrary.h"

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
    if(GetOwner()->Role == ROLE_Authority)
    {
        int bp = 50;
        bp++;
        for(int i = 0; i < m_spellsClass.Num(); ++i)
        {
            USpell* tempSpell;
            tempSpell = NewObject<USpell>(this, m_spellsClass[i]);
            tempSpell->SetIsReplicated(true);

            if(tempSpell)
            {
                tempSpell->initializeSpell();
                m_spells.Emplace(tempSpell);
            }
        }
    }
}


// Called every frame
void USpellKit::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(GetOwner()->Role != ROLE_Authority)
    {
        int bp = 50;
        bp++;
    }
    // ...
}


void USpellKit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(USpellKit, m_spells, COND_OwnerOnly);
}

void USpellKit::cast(int32 index)
{
    if(index < m_spells.Num())
    {
        m_spells[index]->cast();
    }
}

TArray<float> USpellKit::getCooldowns()
{
    TArray<float> cooldowns{};
    for(int32 index{}; index < m_spells.Num(); ++index)
    {
        cooldowns.Emplace(m_spells[index]->getCurrentCooldown());
    }
    return cooldowns;
}