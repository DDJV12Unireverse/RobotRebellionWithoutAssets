// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "HealthPotionActor.h"

void AHealthPotionActor::OnPickup(APawn* InstigatorPawn)
{
    //Nothing. To be derived.
    PRINT_MESSAGE_ON_SCREEN(FColor::Purple, TEXT("Health Potion PickedUp"));
    if (Role == ROLE_Authority)
    Destroy();
}


