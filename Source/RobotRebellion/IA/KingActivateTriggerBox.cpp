// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "KingActivateTriggerBox.h"

#include "Global/EntityDataSingleton.h"
#include "Controller/KingAIController.h"


AKingActivateTriggerBox::AKingActivateTriggerBox()
{
    UShapeComponent* collisionComponent = GetCollisionComponent();
    collisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
    collisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
    collisionComponent->OnComponentHit.AddDynamic(this, &AKingActivateTriggerBox::onHit);
}

void AKingActivateTriggerBox::BeginPlay()
{
    Super::BeginPlay();
}

void AKingActivateTriggerBox::onHit(UPrimitiveComponent* var1, AActor* var2, UPrimitiveComponent* var3, FVector var4, const FHitResult& var5)
{
    AKingAIController* kingController = Cast<AKingAIController>(EntityDataSingleton::getInstance().m_king->GetController());

    if(kingController)
    {
        kingController->activate(true);

        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "COLLISION, KING ACTIVATED");
    }

    this->killItself();
}

void AKingActivateTriggerBox::correctDestruction()
{
    this->Destroy(true);
}

void AKingActivateTriggerBox::killItself()
{
    if (Role < ROLE_Authority)
    {
        this->serverKills();
    }
    else
    {
        this->multiKills();
    }

    this->correctDestruction();
}

void AKingActivateTriggerBox::serverKills_Implementation()
{
    this->multiKills(); 
    this->correctDestruction();
}

bool AKingActivateTriggerBox::serverKills_Validate()
{
    return true;
}

void AKingActivateTriggerBox::multiKills_Implementation()
{
    this->correctDestruction();
}
