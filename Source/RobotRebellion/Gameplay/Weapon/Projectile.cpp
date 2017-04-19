// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Projectile.h"

#include "Character/RobotRebellionCharacter.h"
#include "Gameplay/Damage/Damage.h"
#include "Gameplay/Damage/DamageCoefficientLogic.h"

#include "Global/GlobalDamageMethod.h"
#include "Tool/UtilitaryFunctionLibrary.h"



// Sets default values
AProjectile::AProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create Sphere for collision shape
    m_collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    m_collisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    m_collisionComp->BodyInstance.SetCollisionProfileName("Projectile");

    RootComponent = m_collisionComp;

    //Projectile Movement datas
    m_projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    m_projectileMovement->UpdatedComponent = m_collisionComp;
    m_projectileMovement->InitialSpeed = 3000.f;
    m_projectileMovement->MaxSpeed = 3000.f;
    m_projectileMovement->bRotationFollowsVelocity = true;
    m_projectileMovement->bShouldBounce = false;

    bReplicates = true;
    bNetUseOwnerRelevancy = true;

    //Life Time
    InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProjectile::InitProjectileParams(const FVector& shootDirection, float distanceRange)
{
    if(m_projectileMovement)
    {
        float bulletSpeed = m_projectileMovement->InitialSpeed;

        // Adjust velocity with direction
        m_projectileMovement->Velocity = shootDirection * bulletSpeed;

        //t = d / v
        this->SetLifeSpan(distanceRange / bulletSpeed);
    }
}

void AProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AProjectile, m_owner);
    //DOREPLIFETIME_CONDITION(AProjectile, m_bPressedCrouch, COND_SkipOwner);
}

void AProjectile::OnRep_MyOwner()
{
    setOwner(m_owner);
}

void AProjectile::setOwner(ARobotRebellionCharacter *newOwner)
{
    m_owner = newOwner;
    // Propriétaire réseau pour les appels RPC.
    SetOwner(newOwner);
}

void AProjectile::OnHit(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent*
                        OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    //PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Blue, TEXT("Hit"));
    if(Role == ROLE_Authority)
    {
        this->inflictDamageLogic(OtherActor, Hit);

        Destroy();

        //PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Blue, TEXT("Destroy on Server"));
    }
}

void AProjectile::inflictDamageLogic(AActor* otherActor, const FHitResult& hit)
{
    ARobotRebellionCharacter* receiver = Cast<ARobotRebellionCharacter>(otherActor);
    if(receiver && m_owner != receiver && !receiver->isDead())
    {
        if(!receiver->isImmortal())
        {
            DamageCoefficientLogic coeff;

            Damage damage{ m_owner, receiver };

            Damage::DamageValue currentDamage = damage(
                &UGlobalDamageMethod::normalHitWithWeaponComputed,
                coeff.getCoefficientValue()
            );

            setReceiverInCombat(receiver);
            receiver->inflictDamage(currentDamage);
        }
        //else             // COMMENTED FOR CHEAT CODE
        //{
        //    receiver->displayAnimatedText("IMMORTAL OBJECT", FColor::Purple, ELivingTextAnimMode::TEXT_ANIM_NOT_MOVING);
        //}
    }
}

void AProjectile::setReceiverInCombat(ARobotRebellionCharacter* receiver)
{
    ANonPlayableCharacter * ennemy = Cast<ANonPlayableCharacter>(receiver);
    if(ennemy)
    {
        ACustomAIControllerBase* controller = Cast<ACustomAIControllerBase>(ennemy->GetController());
        if(controller)
        {
            controller->setTarget(m_owner);
        }
    }
}

void AProjectile::simulateInstantRealMethod(const FVector& shootDirection, float distance)
{
    UWorld* world = this->GetWorld();

    if(world)
    {
        FHitResult hitResult;

        FVector start = this->GetActorLocation() + shootDirection * (m_collisionComp->GetScaledSphereRadius() + 0.001f);
        FVector end = start + shootDirection * distance;

        ARobotRebellionCharacter* targetTouched = nullptr;

        if(world->LineTraceSingleByChannel(
            hitResult,
            start,
            end,
            ECC_GameTraceChannel1
        ))
        {
            targetTouched = Cast<ARobotRebellionCharacter>(hitResult.GetActor());

            if(targetTouched)
            {
                this->inflictDamageLogic(targetTouched, hitResult);
            }

            end = hitResult.ImpactPoint;
        }

        this->drawProjectileLineMethod(world, start, end);

        multiDrawLineOnClients(start, end);
    }
}

void AProjectile::simulateInstant(const FVector& shootDirection, float distance)
{
    if (Role < ROLE_Authority)
    {
        serverSimulateInstant(shootDirection, distance);
    }
    else
    {
        simulateInstantRealMethod(shootDirection, distance);
    }

    this->suicide();
}

void AProjectile::serverSimulateInstant_Implementation(const FVector& shootDirection, float distance)
{
    this->simulateInstantRealMethod(shootDirection, distance);
    this->destroyOnClients();
}

bool AProjectile::serverSimulateInstant_Validate(const FVector& shootDirection, float distance)
{
    return true;
}

void AProjectile::drawProjectileLineMethod(UWorld* world, const FVector& start, const FVector& end)
{
    UUtilitaryFunctionLibrary::drawObligatoryPersistentLineInWorld(world, start, end, FColor::White, 1.f, 1.f);
}

void AProjectile::multiDrawLineOnClients_Implementation(const FVector& start, const FVector& end)
{
    this->drawProjectileLineMethod(this->GetWorld(), start, end);
}

void AProjectile::suicide()
{
    this->Destroy(true);
}

void AProjectile::destroyOnClients_Implementation()
{
    this->suicide();
}