// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"

#include "Character/NonPlayableCharacter.h"
#include "SpawnEffect.h"



void USpawnEffect::BeginPlay()
{
    Super::BeginPlay();
}

void USpawnEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpawnEffect::exec(ARobotRebellionCharacter* caster, ARobotRebellionCharacter* target)
{
    // TODO
}

void USpawnEffect::exec(const FVector impactPoint)
{
    // TODO - Spawn the actor!
    // update position
    FVector spawnLocation = impactPoint + m_offsetFromImpactPoint;
    AActor* temp = GetWorld()->SpawnActor<AActor>(m_actorClassToSpawn, spawnLocation, FRotator{0.f});

    if (temp)
    {
        temp->SetLifeSpan(m_actorLifeTime);

        // If actor is a pawn with controller we need to manually spawn it
        if(m_hasDefaultAIController)
        {
            // try to cast to NonPlayableCharacter
            ANonPlayableCharacter* npc = Cast<ANonPlayableCharacter>(temp);
            if(npc)
            {
                npc->SpawnDefaultController();
            }
        }
        TArray<UProjectileMovementComponent*> movementComp;
        temp->GetComponents<UProjectileMovementComponent>(movementComp);
        if(movementComp.Num() > 0) // then we could set initiale speed
        {
            movementComp[0]->MaxSpeed = m_MaxSpeed;
            movementComp[0]->Velocity = m_startSpeed;
            auto test = 0;
            test++;
        }
    }
}