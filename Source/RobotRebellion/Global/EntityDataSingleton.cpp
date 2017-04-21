// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "EntityDataSingleton.h"

#include "Character/RobotRebellionCharacter.h"
#include "Character/PlayableCharacter.h"

#include "Character/Drone.h"
#include "Character/King.h"

#include "IA/Character/SovecCharacter.h"
#include "IA/Character/RobotsCharacter.h"


void EntityDataSingleton::update(const UWorld* world)
{
    m_playableCharacterArray.Reset();
    m_robotArray.Reset();

    if (world)
    {
        TArray<AActor*> foundActors;
        UGameplayStatics::GetAllActorsOfClass(world, ARobotRebellionCharacter::StaticClass(), foundActors);

        for(AActor* current : foundActors)
        {
            if( updateType(current, m_robotArray) ||
                updateType(current, m_playableCharacterArray)
                )
            {
                continue;
            }

            if(!updateType(current, m_king))
            {
                updateType(current, m_drone);
            }
        }
    }
}

void EntityDataSingleton::clean()
{
    m_playableCharacterArray.Reset();
    m_robotArray.Reset();

    m_king = nullptr;
    m_drone = nullptr;
}
