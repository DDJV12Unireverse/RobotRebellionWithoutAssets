// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "WizardMeteor.h"

#include "Character/RobotRebellionCharacter.h"

#include "Gameplay/Damage/Damage.h"
#include "Gameplay/Damage/DamageCoefficientLogic.h"
#include "Global/GlobalDamageMethod.h"

// Sets default values
AWizardMeteor::AWizardMeteor()
    : AActor(),
    m_unreducedDamage{},
    m_explosionRadius{}
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
//    PRINT_MESSAGE_ON_SCREEN(FColor::Purple, "lifeSpan : " + FString::SanitizeFloat(GetLifeSpan()) 
//                            + " - delta : " + FString::SanitizeFloat(delta));
    if(GetLifeSpan() <= delta)
    {
        explode();
    }
}

// Call when actor reach it target location
void AWizardMeteor::explode()
{
    //PRINT_MESSAGE_ON_SCREEN(FColor::Purple, "explode");
    if(Role == ROLE_Authority)
    {
        // Can hit every character
        TArray<TEnumAsByte<EObjectTypeQuery>> objectType = {
            UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2), // Players
            UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3), // Robots
            UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4), // Sovec
            UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6)  // Beasts
        };

		const FVector& actorLocation = GetActorLocation();
        TArray<FHitResult> OutHits;
        const FVector& actorLocation = GetActorLocation();
        UKismetSystemLibrary::SphereTraceMultiForObjects(
            GetWorld(),
			actorLocation,
            actorLocation,
            m_explosionRadius,
            objectType,
            false,
            {this},
            EDrawDebugTrace::ForDuration,
            OutHits,
            true
        );
        {
            for(int32 iter = 0; iter < OutHits.Num(); ++iter)
            {
                FHitResult Hit = OutHits[iter];

                ARobotRebellionCharacter* target = Cast<ARobotRebellionCharacter>(Hit.GetActor());

                if(target)
                {
                    DamageCoefficientLogic coeff;

                    // This line could crash if owner is not RobotRebellion Charcater but it shouldn't
                    // happen...
                    Damage damage{Cast<ARobotRebellionCharacter>(GetOwner()), target};

                    float meteorDamage = m_unreducedDamage;
                    Damage::DamageValue currentDamage = damage(
                        [meteorDamage](const ARobotRebellionCharacter* caster, const ARobotRebellionCharacter* target)
                    {
                        // use strenght and shield cause it's fun to use shield
                        int32 ratio = (caster->getStrength() + (caster->getShield() * 2.f)) / (target->getDefense());
                        return static_cast<Damage::DamageValue>(ratio * meteorDamage);
                    }, 1.0f);

                    target->inflictDamage(currentDamage);
                }
            }
        }
    }

    // now destroye the actor
    this->Destroy();
}

void AWizardMeteor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AWizardMeteor::setCaster(ARobotRebellionCharacter* p)
{
    m_caster = p;
}

