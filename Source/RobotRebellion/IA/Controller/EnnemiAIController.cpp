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
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void AEnnemiAIController::CheckEnnemyNear(float range)
{
    if (!m_showDebugSphereTrace)
    {
        EntityDataSingleton& sing = EntityDataSingleton::getInstance();
        m_targetToFollow = nullptr;

        float rangeSquared = range * range;
        FVector currentLocation = GetPawn()->GetActorLocation();

        float distSquaredTemp;
        float minDist = 9e+34f;
        for(APlayableCharacter* player : sing.m_playableCharacterArray)
        {
            if(player->isVisible() && !player->isDead())
            {
                distSquaredTemp = FVector::DistSquared(currentLocation, player->GetActorLocation());
                if(distSquaredTemp < rangeSquared)
                {
                    minDist = distSquaredTemp;
                    m_targetToFollow = player;
                }
            }
        }

        {
            AKing* king = sing.m_king;
            distSquaredTemp = FVector::DistSquared(currentLocation, king->GetActorLocation());
            if(distSquaredTemp < minDist)
            {
                minDist = distSquaredTemp;
                m_targetToFollow = king;
            }
        }

        for(ASovecCharacter* sovec : sing.m_sovecArray)
        {
            distSquaredTemp = FVector::DistSquared(currentLocation, sovec->GetActorLocation());
            if(distSquaredTemp < minDist)
            {
                minDist = distSquaredTemp;
                m_targetToFollow = sovec;
                break;
            }
        }

        for(ABeastCharacter* beast : sing.m_beastArray)
        {
            if(FVector::DistSquared(currentLocation, beast->GetActorLocation()) < minDist)
            {
                m_targetToFollow = beast;
                break;
            }
        }
    }
    else
    {
        this->formerCheckMethod(range);
    }
}

void AEnnemiAIController::formerCheckMethod(float range)
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

void AEnnemiAIController::AttackTarget() const
{
    ANonPlayableCharacter* ennemiCharacter = Cast<ANonPlayableCharacter>(GetCharacter());
    if(m_targetToFollow)
    {
        ennemiCharacter->m_weaponInventory->getCurrentWeapon()->cppAttack(ennemiCharacter, m_targetToFollow);
    }
    else
    {
        ennemiCharacter->m_weaponInventory->getCurrentWeapon()->cppAttack(ennemiCharacter);
    }
}