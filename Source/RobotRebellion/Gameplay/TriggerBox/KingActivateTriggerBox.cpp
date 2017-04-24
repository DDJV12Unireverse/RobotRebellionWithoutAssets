// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "KingActivateTriggerBox.h"

#include "Global/EntityDataSingleton.h"
#include "IA/Controller/KingAIController.h"
#include "Character/King.h"


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
    if(Role >= ROLE_Authority)
    {
        this->internal_signalToServer();
    }
    else
    {
        signalToServer();
    }


    this->killItself();
}

void AKingActivateTriggerBox::internal_signalToServer()
{
    AKing* king = EntityDataSingleton::getInstance().getServerKing(this);

    if(king)
    {
        AKingAIController* kingController = Cast<AKingAIController>(king->GetController());

        check(kingController);

        kingController->activate(true);

        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "COLLISION, KING ACTIVATED");
    }
}

void AKingActivateTriggerBox::signalToServer_Implementation()
{
    this->internal_signalToServer();
}

bool AKingActivateTriggerBox::signalToServer_Validate()
{
    return true;
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
