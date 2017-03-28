// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Kaboom.h"

#include "Character/RobotRebellionCharacter.h"
#include "Character/Drone.h"

#include "Gameplay/Damage/Damage.h"
#include "Gameplay/Damage/DamageCoefficientLogic.h"
#include "Global/GlobalDamageMethod.h"

#include "Kismet/KismetSystemLibrary.h"


AKaboom::AKaboom()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create Sphere for collision shape
    m_collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    m_collisionComp->OnComponentHit.AddDynamic(this, &AKaboom::onHit);
    m_collisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    m_collisionComp->InitSphereRadius(5.0f);

    RootComponent = m_collisionComp;

    bReplicates = true;
    bNetUseOwnerRelevancy = true;

    //Life Time
    InitialLifeSpan = 10.0f;

    m_objectTypesToConsider.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)); // Players
    m_objectTypesToConsider.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4)); // Sovec
    m_objectTypesToConsider.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6)); // Beasts
}

void AKaboom::BeginPlay()
{
    Super::BeginPlay();

    this->deactivateBomb();
}

void AKaboom::attachToDrone(ADrone* drone)
{
    this->AttachRootComponentToActor(drone);
}

void AKaboom::detonationImplementation()
{
    FVector actorLocation = GetActorLocation();
    
    TArray<FHitResult> OutHits;

    ARobotRebellionCharacter* drone = Cast<ARobotRebellionCharacter>(GetParentActor());
    
    if(drone && UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetWorld(),
        actorLocation,
        actorLocation,
        m_detonationRadius,
        m_objectTypesToConsider,
        false,
        { this },
        EDrawDebugTrace::ForDuration,
        OutHits,
        true
    ))
    {
        for(int32 iter = 0; iter < OutHits.Num(); ++iter)
        {
            FHitResult Hit = OutHits[iter];

            ARobotRebellionCharacter* targetInDistress = Cast<ARobotRebellionCharacter>(Hit.GetActor());
            
            if(targetInDistress)
            {
                DamageCoefficientLogic coeff;

                Damage damage{ drone, targetInDistress };

                Damage::DamageValue currentDamage = damage(
                    &UGlobalDamageMethod::droneDamageComputed,
                    coeff.getCoefficientValue()
                );

                targetInDistress->inflictDamage(currentDamage + m_baseDamage);
            }
        }
    }

    this->Destroy();
}