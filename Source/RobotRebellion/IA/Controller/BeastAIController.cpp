// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "BeastAIController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Character/NonPlayableCharacter.h"
#include "Gameplay/Weapon/WeaponInventory.h"
#include "Gameplay/Weapon/WeaponBase.h"


void ABeastAIController::CheckEnnemyNear(float range)
{
    APawn *currentPawn = GetPawn();
    FVector MultiSphereStart = currentPawn->GetActorLocation();
    FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, 15.0f);
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)); // Players
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3)); // Robots
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4)); // Sovec
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

void ABeastAIController::AttackTarget() const
{
    ANonPlayableCharacter* ennemiCharacter = Cast<ANonPlayableCharacter>(GetCharacter());
    if(ennemiCharacter)
    {
        ennemiCharacter->m_weaponInventory->getCurrentWeapon()->cppAttack(ennemiCharacter);        
    }
}