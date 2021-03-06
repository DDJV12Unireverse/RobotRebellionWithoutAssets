// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "GunTurretAIController.h"

#include "Character/NonPlayableCharacter.h"
#include "IA/Character/SovecCharacter.h"
#include "IA/Character/BeastCharacter.h"
#include "IA/Character/RobotsCharacter.h"
#include "Gameplay/Weapon/WeaponInventory.h"
#include "Gameplay/Weapon/WeaponBase.h"
#include "Global/EntityDataSingleton.h"

void AGunTurretAIController::CheckEnnemyNear(float range)
{
    APawn *currentPawn = GetPawn();
    FVector MultiSphereStart = currentPawn->GetActorLocation();
    FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, 15.0f);
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6)); // Beast
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3)); // Robots
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(currentPawn);
    TArray<FHitResult> OutHits;
    bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
        MultiSphereStart,
        MultiSphereEnd,
        range,
        ObjectTypes,
        false,
        ActorsToIgnore,
        this->debugDrawTraceShowingMode(),
        OutHits,
        true);

    if(Result == true)
    {
        for(int32 i = 0; i < OutHits.Num(); i++)
        {
            FHitResult Hit = OutHits[i];
            ARobotRebellionCharacter* RRCharacter = Cast<ARobotRebellionCharacter>(Hit.GetActor());
            if(NULL != RRCharacter)
            {
                if(RRCharacter->isDead() || !RRCharacter->isVisible())
                {
                    continue;
                }
                setTarget(RRCharacter);
                break;
            }
        }
    }
    else
    {
        setTarget(nullptr);
    }
}

void AGunTurretAIController::AttackTarget() const
{
    ANonPlayableCharacter* ennemiCharacter = Cast<ANonPlayableCharacter>(GetCharacter());
    ennemiCharacter->m_weaponInventory->getCurrentWeapon()->cppAttack(ennemiCharacter);
}