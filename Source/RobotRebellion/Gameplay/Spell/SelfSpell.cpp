// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "SelfSpell.h"
#include "Effect.h"
#include "../../Character/RobotRebellionCharacter.h"

USelfSpell::USelfSpell() : USpell()
{

}

void USelfSpell::BeginPlay()
{
    Super::BeginPlay();
}

void USelfSpell::cast()
{
    if(!canCast())
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Emerald,
                                "Cooldown : " + FString::FromInt(m_nextAllowedCastTimer - FPlatformTime::Seconds()));
        return;
    }

    ARobotRebellionCharacter* caster = Cast<ARobotRebellionCharacter>(GetOwner());
    if(caster)
    {
        applyEffect(caster);

        // the spell is successfully cast consumme mana and launch CD
        caster->consumeMana(m_manaCost);
        m_nextAllowedCastTimer = FPlatformTime::Seconds() + m_cooldown;
    }
}

void USelfSpell::applyEffect(ARobotRebellionCharacter* affectedTarget)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, TEXT("ApplicateEffect on target"));
    for(int i = 0; i < m_effects.Num(); ++i)
    {
        m_effects[i]->exec(Cast<ARobotRebellionCharacter>(GetOwner()), affectedTarget);
    }
}
