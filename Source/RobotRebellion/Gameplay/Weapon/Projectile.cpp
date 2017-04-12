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

void AProjectile::InitVelocity(const FVector& shootDirection)
{
    if(m_projectileMovement)
    {
        // Adjust velocity with direction
        m_projectileMovement->Velocity = shootDirection * m_projectileMovement->InitialSpeed;
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
    // Propri�taire r�seau pour les appels RPC.
    SetOwner(newOwner);
}

void AProjectile::OnHit(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent*
                        OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    //PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Blue, TEXT("Hit"));
    if(Role == ROLE_Authority)
    {
        ARobotRebellionCharacter* receiver = Cast<ARobotRebellionCharacter>(OtherActor);
        if(receiver && m_owner != receiver && !receiver->isDead())
        {
            if(!receiver->isImmortal())
            {
                DamageCoefficientLogic coeff;

                Damage damage{m_owner, receiver};

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

        Destroy();

        //PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Blue, TEXT("Destroy on Server"));
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
