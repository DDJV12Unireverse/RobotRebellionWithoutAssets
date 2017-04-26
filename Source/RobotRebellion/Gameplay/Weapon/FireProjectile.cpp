// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "FireProjectile.h"
#include "Character/RobotRebellionCharacter.h"
#include "Global/WorldInstanceEntity.h"


AFireProjectile::AFireProjectile()
{

}


void AFireProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void AFireProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFireProjectile::OnHit(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ARobotRebellionCharacter* receiver = Cast<ARobotRebellionCharacter>(OtherActor);
    if(receiver && receiver !=m_owner)
    {
            receiver->spawnFireEffect(Hit.Location);
    }
    if(Role == ROLE_Authority)
    {
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
                receiver->inflictDamage(m_baseDamage*currentDamage);
            }
        }
        

        //TODO display burn effect on Mesh
        //ASurvivalCharacter* hitActor = Cast<ASurvivalCharacter>(Impact.GetActor());
        
        Destroy();
    }
}