// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ThrowSpell.h"


void UThrowSpell::cast()
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, TEXT("Cast Throw Spell"));
}

void UThrowSpell::onHit(UPrimitiveComponent*, AActor* target, UPrimitiveComponent*, FVector, const FHitResult& hitResult)
{
    if(m_isTargetThrow)
    {
        ARobotRebellionCharacter* hitChar = Cast<ARobotRebellionCharacter>(target);
        if(hitChar)
        {
            applicateEffect(hitChar);
        }
    }
    else
    {
        applicateEffect(hitResult.ImpactPoint);
    }
}

void UThrowSpell::applicateEffect(ARobotRebellionCharacter* affectedTarget)
{
}

void UThrowSpell::applicateEffect(FVector impactPoint)
{
}
