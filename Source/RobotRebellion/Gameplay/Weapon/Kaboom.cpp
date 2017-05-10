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

    m_explosionPCS = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion PCS"));

    m_explosionPCS->bAutoActivate = false;
    m_explosionPCS->bAutoDestroy = false;
    m_explosionPCS->SetupAttachment(RootComponent);

    bReplicates = true;
    bNetUseOwnerRelevancy = true;

    m_destroyMethod = &AKaboom::noMethod;

    this->initializeDamagedObjectList();
    this->initializeKaboomMovementComponent();
    this->deactivateBomb();
}

void AKaboom::BeginPlay()
{
    Super::BeginPlay();

}

void AKaboom::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    (this->*m_destroyMethod)();
}

void AKaboom::initializeDamagedObjectList()
{
    m_objectTypesToConsider = {
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2), // Players
        UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3), // Robots
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
    m_collisionComp->SetAllPhysicsAngularVelocity(FVector::ZeroVector);
    m_collisionComp->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
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
    if(Role < ROLE_Authority)
    {
        return;
    }

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
        SPHERECAST_DISPLAY_DURATION,
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
    UGameplayStatics::SpawnSoundAttached(m_boomSound, GetRootComponent());

    multiExplosionOnEveryone();
}

void AKaboom::realDestroy()
{
    m_explosionPCS->DeactivateSystem();
    m_explosionPCS->DestroyComponent();

    this->BeginDestroy();
}

void AKaboom::multiExplosionOnEveryone_Implementation()
{
    this->deactivateBomb();
    m_destroyMethod = &AKaboom::realDestroy;

    this->m_kaboomMesh->SetVisibility(false);

    UGameplayStatics::SpawnSoundAttached(m_boomSound, GetRootComponent());

    m_explosionPCS->SetRelativeScale3D(m_explosionEffectScale);

    m_explosionPCS->ActivateSystem();
}