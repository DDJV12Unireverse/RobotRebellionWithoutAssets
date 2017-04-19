// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Projectile.h"

#include "Character/RobotRebellionCharacter.h"
#include "../Damage/Damage.h"
#include "../Damage/DamageCoefficientLogic.h"

#include "../../Global/GlobalDamageMethod.h"
#include "../../Tool/UtilitaryFunctionLibrary.h"
#include "IA/Controller/CustomAIControllerBase.h"
#include "Character/NonPlayableCharacter.h"



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
        TArray<FHitResult> hitResult;

        FVector start = this->GetActorLocation();

        if(world->LineTraceMultiByChannel(
            hitResult,
            start + shootDirection * m_collisionComp->GetScaledSphereRadius() * 1.5f,
            start + shootDirection * distance,
            ECC_GameTraceChannel1
        ))
        {
            hitResult.Sort([](const FHitResult& hit1, const FHitResult& hit2) {return hit1.Distance < hit2.Distance; });

            FHitResult& firstHit = hitResult[0];

            ARobotRebellionCharacter* targetTouched = Cast<ARobotRebellionCharacter>(firstHit.GetActor());

            if(targetTouched)
            {
                this->inflictDamageLogic(targetTouched, firstHit);
            }
        }
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

void AProjectile::suicide()
{
    this->Destroy(true);
}

void AProjectile::destroyOnClients_Implementation()
{
    this->suicide();
}