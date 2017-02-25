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



void URobotRobellionSpawnerClass::spawnAndReplace(ARobotRebellionCharacter* owner, EClassType typeToChange)
{
    if (owner->Role < ROLE_Authority)
    {
        serverSpawnAndReplace(owner, typeToChange);
    }
    else if (typeToChange != owner->getType()) //already the same, does nothing
    {
        UWorld* const world = owner->GetWorld();
        if (world)
        {
            FActorSpawnParameters spawnParams;
            spawnParams.Owner = owner->GetOwner();
            spawnParams.Instigator = owner->Instigator;
            spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            
            ARobotRebellionCharacter* intermediary;

            FTransform spawnTransform{
                FRotator{0.f,0.f,0.f},
                owner->GetActorLocation(),
                owner->GetActorScale3D()
            };

            switch (typeToChange)
            {
            case EClassType::ASSASSIN:
            {
                intermediary = world->SpawnActor<AAssassin>(m_assassinActor, spawnTransform, spawnParams);
            }
            break;

            case EClassType::HEALER:
            {
                intermediary = world->SpawnActor<AHealer>(m_healerActor, spawnTransform, spawnParams);
            }
            break;

            case EClassType::SOLDIER:
            {
                intermediary = world->SpawnActor<ASoldier>(m_soldierActor, spawnTransform, spawnParams);
            }
            break;

            case EClassType::WIZARD:
            {
                intermediary = world->SpawnActor<AWizard>(m_wizardActor, spawnTransform, spawnParams);
            }
            break;

            default:
                return;
            }

            owner->Controller->Possess(intermediary);

            owner->cppOnRevive();

            owner->Destroy();

            GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Black, "Spawn");
        }
    }
}

void URobotRobellionSpawnerClass::serverSpawnAndReplace_Implementation(ARobotRebellionCharacter* owner, EClassType typeToChange)
{
    spawnAndReplace(owner, typeToChange);
}

bool URobotRobellionSpawnerClass::serverSpawnAndReplace_Validate(ARobotRebellionCharacter* owner, EClassType typeToChange)
{
    return true;
}