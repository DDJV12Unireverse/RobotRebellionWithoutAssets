// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotRobellionSpawnerClass.h"

#include "RobotRebellionCharacter.h"
#include "Assassin.h"
#include "Wizard.h"
#include "Healer.h"
#include "Soldier.h"


// Sets default values for this component's properties
URobotRobellionSpawnerClass::URobotRobellionSpawnerClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URobotRobellionSpawnerClass::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URobotRobellionSpawnerClass::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}



void URobotRobellionSpawnerClass::spawnAndReplace(EClassType type, ARobotRebellionCharacter* original)
{
    if (type != original->getType()) //already the same, does nothing
    {
        UWorld* const world = original->GetWorld();
        if (world)
        {
            FActorSpawnParameters spawnParams;
            spawnParams.Owner = original->GetOwner();
            spawnParams.Instigator = original->Instigator;
            spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            ARobotRebellionCharacter* intermediary;

            switch (type)
            {
            case EClassType::ASSASSIN:
            {
                intermediary = world->SpawnActor<AAssassin>(m_assassinActor, original->GetTransform(), spawnParams);
            }
            break;

            case EClassType::HEALER:
            {
                intermediary = world->SpawnActor<AHealer>(m_healerActor, original->GetTransform(), spawnParams);
            }
            break;

            case EClassType::SOLDIER:
            {
                intermediary = world->SpawnActor<ASoldier>(m_soldierActor, original->GetTransform(), spawnParams);
            }
            break;

            case EClassType::WIZARD:
            {
                intermediary = world->SpawnActor<AWizard>(m_wizardActor, original->GetTransform(), spawnParams);
            }
            break;

            default:
                return;
            }
            GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Black, "Spawn");
            intermediary->Controller = original->GetController();
            original = intermediary;
        }
    }
}