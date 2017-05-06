// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CorridorTriggerBox.h"
#include "Character/RobotRebellionCharacter.h"




ACorridorTriggerBox::ACorridorTriggerBox()
{
    //GetCollisionComponent()->OnComponentHit.AddDynamic(this, &ABigRoomTriggerBox::onOverlapBegin);
    GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACorridorTriggerBox::onOverlapBegin);
}

void ACorridorTriggerBox::BeginPlay()
{
    Super::BeginPlay();
}


void ACorridorTriggerBox::onOverlapBegin(UPrimitiveComponent* var1, AActor* var2, UPrimitiveComponent* var3,
                                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ARobotRebellionCharacter* player = Cast<ARobotRebellionCharacter>(var2);
    if(player)
    {
        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "TRIGGER CORRIDOR");
        player->setLocation(ELocation::CORRIDOR);
    }

}



