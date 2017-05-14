// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "OutsideTriggerBox.h"
#include "Character/RobotRebellionCharacter.h"




AOutsideTriggerBox::AOutsideTriggerBox()
{
    //GetCollisionComponent()->OnComponentHit.AddDynamic(this, &ABigRoomTriggerBox::onOverlapBegin);
    GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AOutsideTriggerBox::onOverlapBegin);
}

void AOutsideTriggerBox::BeginPlay()
{
    Super::BeginPlay();
}


void AOutsideTriggerBox::onOverlapBegin(UPrimitiveComponent* var1, AActor* var2, UPrimitiveComponent* var3,
                                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ARobotRebellionCharacter* player = Cast<ARobotRebellionCharacter>(var2);
    if(player)
    {
        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "TRIGGER OUTSIDE");
        player->setLocation(ELocation::OUTSIDE);
    }

}





