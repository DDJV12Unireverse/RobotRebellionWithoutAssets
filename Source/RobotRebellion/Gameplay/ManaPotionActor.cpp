// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ManaPotionActor.h"

void AManaPotionActor::OnPickup(APawn* InstigatorPawn)
{
    //Nothing. To be derived.
    PRINT_MESSAGE_ON_SCREEN(FColor::Purple, TEXT("Mana potion PickedUp"));
    if (Role == ROLE_Authority)
    {
        Destroy();
    }
}


