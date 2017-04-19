// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "BossActivateTriggerBox.h"
#include "../Global/RobotRebellionGameMode.h"

ABossActivateTriggerBox::ABossActivateTriggerBox()
{
	GetCollisionComponent()->OnComponentHit.AddDynamic(this, &ABossActivateTriggerBox::onHit);
}

void ABossActivateTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}


void ABossActivateTriggerBox::onHit(UPrimitiveComponent* var1, AActor* var2, UPrimitiveComponent* var3, FVector var4, const FHitResult& var5)
{

	PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "COLLISION, BOSS SOUND ACTIVATED");

	UWorld* w = this->GetWorld();
    ARobotRebellionGameState* gameState = Cast<ARobotRebellionGameState>(w->GetGameState());
	if(gameState)
	{
		gameState->setBossGameMode();
	}

	this->killItself();
}

void ABossActivateTriggerBox::correctDestruction()
{
	this->Destroy(true);
}

void ABossActivateTriggerBox::killItself()
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

void ABossActivateTriggerBox::serverKills_Implementation()
{
	this->multiKills();
	this->correctDestruction();
}

bool ABossActivateTriggerBox::serverKills_Validate()
{
	return true;
}

void ABossActivateTriggerBox::multiKills_Implementation()
{
	this->correctDestruction();
}
