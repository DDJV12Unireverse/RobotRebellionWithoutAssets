// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "BombActor.h"



void ABombActor::OnPickup(APawn* InstigatorPawn)
{
    //Nothing. To be derived.
    PRINT_MESSAGE_ON_SCREEN(FColor::Purple, TEXT("Bomb PickedUp"));

    Destroy();
}

