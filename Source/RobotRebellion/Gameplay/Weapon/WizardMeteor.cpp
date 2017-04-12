// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "WizardMeteor.h"


// Sets default values
AWizardMeteor::AWizardMeteor()
    : AActor(),
    m_unreducedDamage{}
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //Projectile Movement datas
    m_projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Comp"));
    m_projectileMovement->InitialSpeed = 3000.f;
    m_projectileMovement->MaxSpeed = 3000.f;
    m_projectileMovement->bRotationFollowsVelocity = true;
    m_projectileMovement->bShouldBounce = false;

    bReplicates = true;
}

// Called when the game starts or when spawned
void AWizardMeteor::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AWizardMeteor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //Use life Time 
    float delta = 0.5f;
    PRINT_MESSAGE_ON_SCREEN(FColor::Purple, "lifeSpan : " + FString::SanitizeFloat(GetLifeSpan()) 
                            + " - delta : " + FString::SanitizeFloat(delta));
    if(GetLifeSpan() <= delta)
    {
        explode();
    }
}

// Call when actor reach it target location
void AWizardMeteor::explode()
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Purple, "explode");
    if(Role == ROLE_Authority)
    {
        // TODO - Explode and deals damage to every character hit
    }

    // now destroye the actor
    this->Destroy();
}

void AWizardMeteor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

