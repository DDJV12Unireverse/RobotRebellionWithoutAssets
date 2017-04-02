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
    if(!m_showDebugSphereTrace)
    {
        EntityDataSingleton& sing = EntityDataSingleton::getInstance();

        float rangeSquared = range * range;
        FVector currentLocation = GetPawn()->GetActorLocation();

        for(ARobotsCharacter* robots : sing.m_robotArray)
        {
            if(FVector::DistSquared(currentLocation, robots->GetActorLocation()) < rangeSquared)
            {
                m_targetToFollow = robots;
                return;
            }
        }

        for(ABeastCharacter* beast : sing.m_beastArray)
        {
            if(FVector::DistSquared(currentLocation, beast->GetActorLocation()) < rangeSquared)
            {
                m_targetToFollow = beast;
                return;
            }
        }
    }
    else
    {
        this->formerCheckMethod(range);
    }
}

void AGunTurretAIController::formerCheckMethod(float range)
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

    m_targetToFollow = NULL;

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
                m_targetToFollow = RRCharacter;
                break;
            }
        }
    }
}

void AGunTurretAIController::AttackTarget() const
{
    ANonPlayableCharacter* ennemiCharacter = Cast<ANonPlayableCharacter>(GetCharacter());
    ennemiCharacter->m_weaponInventory->getCurrentWeapon()->cppAttack(ennemiCharacter);
}