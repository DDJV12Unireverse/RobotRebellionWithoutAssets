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



void URobotRobellionSpawnerClass::spawnAndReplace(ARobotRebellionCharacter* m_owner, EClassType m_typeToChange)
{
    if (m_owner->Role < ROLE_Authority)
    {
        serverSpawnAndReplace(m_owner, m_typeToChange);
    }
    else if (m_typeToChange != m_owner->getType()) //already the same, does nothing
    {
        UWorld* const world = m_owner->GetWorld();
        if (world)
        {
            FActorSpawnParameters spawnParams;
            spawnParams.Owner = m_owner->GetOwner();
            spawnParams.Instigator = m_owner->Instigator;
            spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            
            ARobotRebellionCharacter* intermediary;

            switch (m_typeToChange)
            {
            case EClassType::ASSASSIN:
            {
                intermediary = world->SpawnActor<AAssassin>(m_assassinActor, m_owner->GetTransform(), spawnParams);
            }
            break;

            case EClassType::HEALER:
            {
                intermediary = world->SpawnActor<AHealer>(m_healerActor, m_owner->GetTransform(), spawnParams);
            }
            break;

            case EClassType::SOLDIER:
            {
                intermediary = world->SpawnActor<ASoldier>(m_soldierActor, m_owner->GetTransform(), spawnParams);
            }
            break;

            case EClassType::WIZARD:
            {
                intermediary = world->SpawnActor<AWizard>(m_wizardActor, m_owner->GetTransform(), spawnParams);
            }
            break;

            default:
                return;
            }

            m_owner->Controller->Possess(intermediary);

            m_owner->Destroy();

            GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Black, "Spawn");
        }
    }
}

void URobotRobellionSpawnerClass::serverSpawnAndReplace_Implementation(ARobotRebellionCharacter* m_owner, EClassType m_typeToChange)
{
    spawnAndReplace(m_owner, m_typeToChange);
}

bool URobotRobellionSpawnerClass::serverSpawnAndReplace_Validate(ARobotRebellionCharacter* m_owner, EClassType m_typeToChange)
{
    return true;
}