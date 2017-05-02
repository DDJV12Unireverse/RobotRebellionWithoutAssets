// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "SmallRoomTriggerBox.h"
#include "Character/PlayableCharacter.h"




ASmallRoomTriggerBox::ASmallRoomTriggerBox()
{
    //GetCollisionComponent()->OnComponentHit.AddDynamic(this, &ABigRoomTriggerBox::onOverlapBegin);
    GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASmallRoomTriggerBox::onOverlapBegin);
}

void ASmallRoomTriggerBox::BeginPlay()
{
    Super::BeginPlay();
}


void ASmallRoomTriggerBox::onOverlapBegin(UPrimitiveComponent* var1, AActor* var2, UPrimitiveComponent* var3,
                                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APlayableCharacter* player = Cast<APlayableCharacter>(var2);
    if(player)
    {
        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "TRIGGER SMALLROOM");
        player->setLocation(ELocation::OUTSIDE);
    }

}




