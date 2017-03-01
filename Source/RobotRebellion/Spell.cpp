// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Spell.h"


USpell::USpell()
{
    // TODO
}

void USpell::cast()
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, FString::Printf(TEXT("Spell cast : %s "), *m_name));
}
