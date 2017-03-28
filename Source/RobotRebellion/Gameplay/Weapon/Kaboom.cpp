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

    m_kaboomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Kaboom Mesh"));
    m_kaboomMesh->SetupAttachment(m_collisionComp);

    RootComponent = m_collisionComp;

    bReplicates = true;
    bNetUseOwnerRelevancy = true;

    this->initializeDamagedObjectList();
    this->initializeKaboomMovementComponent();
}

void AKaboom::BeginPlay()
{
    Super::BeginPlay();

    this->deactivateBomb();
}

void AKaboom::initializeDamagedObjectList()
{
    m_objectTypesToConsider = {
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2), // Players
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4), // Sovec
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6)  // Beasts
    };
}

void AKaboom::initializeKaboomMovementComponent()
{
    m_kaboomMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Kaboom Movement"));
    m_kaboomMovement->UpdatedComponent = m_collisionComp;
    
    m_kaboomMovement->InitialSpeed = 0.f;
    m_kaboomMovement->MaxSpeed = 1000.f;
    m_kaboomMovement->bRotationFollowsVelocity = true;
    m_kaboomMovement->bShouldBounce = false;
    m_kaboomMovement->Bounciness = 0.f;
}

void AKaboom::dropingPhysicSetting(bool reenablePhysic)
{
    m_collisionComp->SetSimulatePhysics(reenablePhysic);
    m_collisionComp->SetEnableGravity(reenablePhysic);
}

void AKaboom::attachToDrone(ADrone* drone)
{
    if (drone)
    {
        this->AttachToActor(drone, FAttachmentTransformRules::KeepRelativeTransform);
        this->dropingPhysicSetting(false);
        m_linkedDrone = drone;
    }
}

void AKaboom::detachFromDrone()
{
    if (m_linkedDrone)
    {
        this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        this->dropingPhysicSetting(true);
    }
}

void AKaboom::detonationImplementation()
{
    FVector actorLocation = GetActorLocation();
    
    TArray<FHitResult> OutHits;

    if(m_linkedDrone && UKismetSystemLibrary::SphereTraceMultiForObjects(
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

                Damage damage{ m_linkedDrone, targetInDistress };

                Damage::DamageValue currentDamage = damage(
                    &UGlobalDamageMethod::droneDamageComputed,
                    coeff.getCoefficientValue()
                );

                targetInDistress->inflictDamage(currentDamage + m_baseDamage);
            }
        }
    }

    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "KABOOOOOOOOOOOOOOOOOOUUUUUUUUMMMMMMMMMM !!!!!");

    this->Destroy();
}
