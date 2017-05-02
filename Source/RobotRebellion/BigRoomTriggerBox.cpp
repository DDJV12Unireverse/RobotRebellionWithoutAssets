// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "BigRoomTriggerBox.h"
#include "Character/PlayableCharacter.h"




ABigRoomTriggerBox::ABigRoomTriggerBox()
{
    //GetCollisionComponent()->OnComponentHit.AddDynamic(this, &ABigRoomTriggerBox::onOverlapBegin);
    GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABigRoomTriggerBox::onOverlapBegin);
}

void ABigRoomTriggerBox::BeginPlay()
{
    Super::BeginPlay();
}


void ABigRoomTriggerBox::onOverlapBegin(UPrimitiveComponent* var1, AActor* var2, UPrimitiveComponent* var3,
                                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


    APlayableCharacter* player = Cast<APlayableCharacter>(var2);
    if(player)
    {
        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "TRIGGER BIGROOM");
        player->setLocation(ELocation::BIGROOM);
    }
    
}

