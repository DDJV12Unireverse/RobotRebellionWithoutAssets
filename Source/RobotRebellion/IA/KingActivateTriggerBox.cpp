// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "KingActivateTriggerBox.h"

#include "Global/EntityDataSingleton.h"
#include "Controller/KingAIController.h"


AKingActivateTriggerBox::AKingActivateTriggerBox()
{
    GetCollisionComponent()->OnComponentHit.AddDynamic(this, &AKingActivateTriggerBox::onHit);
}

void AKingActivateTriggerBox::onHit(UPrimitiveComponent* var1, AActor* var2, UPrimitiveComponent* var3, FVector var4, const FHitResult& var5)
{
    AKingAIController* kingController = Cast<AKingAIController>(EntityDataSingleton::getInstance().m_king->GetController());

    if (kingController)
    {
        kingController->activate(true);
    }

    this->Destroy();
}