// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "EnnemiAIController.h"
#include "Character/RobotRebellionCharacter.h"
#include "Character/NonPlayableCharacter.h"
#include "Character/PlayableCharacter.h"
#include "Character/King.h"
#include "IA/Character/SovecCharacter.h"
#include "IA/Character/BeastCharacter.h"
#include "Gameplay/Weapon/WeaponInventory.h"
#include "Gameplay/Weapon/WeaponBase.h"
#include "Global/EntityDataSingleton.h"
#include "IA/Character/RobotsCharacter.h"
#include "AIController.h"
#include "Tool/UtilitaryMacros.h"

#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
//#include "UnrealString.h"

void AEnnemiAIController::CheckEnnemyNear(float range)
{
    APawn *currentPawn = GetPawn();

    FVector MultiSphereStart = currentPawn->GetActorLocation();
    FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, 15.0f);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)); // Players
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4)); // Sovec
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6)); // Beasts

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(currentPawn);

    m_targetToFollow = NULL;

    TArray<FHitResult> OutHits;
    if(UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
                                                        MultiSphereStart,
                                                        MultiSphereEnd,
                                                        range,
                                                        ObjectTypes,
                                                        false,
                                                        ActorsToIgnore,
                                                        this->debugDrawTraceShowingMode(),
                                                        OutHits,
                                                        true))
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

void AEnnemiAIController::AttackTarget() const
{
    ANonPlayableCharacter* ennemiCharacter = Cast<ANonPlayableCharacter>(GetCharacter());
    if(m_targetToFollow)
    {
        FVector hitDirection = UKismetMathLibrary::GetForwardVector(
            UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), m_targetToFollow->GetActorLocation()));
        hitDirection.Z = 0;
        hitDirection.Normalize();
        FVector front = GetPawn()->GetActorForwardVector();
        front.Z = 0;
        front.Normalize();
        FVector vert = FVector::CrossProduct(front, hitDirection);
        float moveDirection = vert.Z;
        float sinAngle = vert.Size();

         if(moveDirection > 0)
         {
             GetPawn()->AddActorLocalRotation(FQuat(FVector(0, 0, 1), asinf(sinAngle)), true); //Correct
         }
         else
         {
             GetPawn()->AddActorLocalRotation(FQuat(FVector(0, 0, 1), asinf(-sinAngle)), true);
         }
        
        
        ennemiCharacter->m_weaponInventory->getCurrentWeapon()->cppAttack(ennemiCharacter, m_targetToFollow);
    }
    else
    {
        ennemiCharacter->m_weaponInventory->getCurrentWeapon()->cppAttack(ennemiCharacter);
    }
}
