// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "FireProjectile.h"
#include "Character/RobotRebellionCharacter.h"


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
    if (Role == ROLE_Authority)
    {
        ARobotRebellionCharacter* receiver = Cast<ARobotRebellionCharacter>(OtherActor);
        if (receiver && m_owner != receiver && !receiver->isDead())
        {
            if (!receiver->isImmortal())
            {
                DamageCoefficientLogic coeff;

                /*UUtilitaryFunctionLibrary::randomApplyObjectMethod<1>(
                true,
                coeff,
                &DamageCoefficientLogic::criticalHit,
                &DamageCoefficientLogic::engagementHit,
                &DamageCoefficientLogic::superEfficient,
                &DamageCoefficientLogic::lessEfficient,
                &DamageCoefficientLogic::multipleHit,
                &DamageCoefficientLogic::graze
                );

                PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Cyan, FString::Printf(TEXT("Coefficient value at : %f"), coeff.getCoefficientValue()));*/

                Damage damage{ m_owner, receiver };

                Damage::DamageValue currentDamage = damage(
                    &UGlobalDamageMethod::normalHitWithWeaponComputed,
                    coeff.getCoefficientValue()
                );

                receiver->inflictDamage(m_baseDamage*currentDamage);
            }
            else
            {
                receiver->displayAnimatedText("IMMORTAL OBJECT", FColor::Purple, ELivingTextAnimMode::TEXT_ANIM_NOT_MOVING);
            }
        }

        //TODO display burn effect on Mesh

        Destroy();
    }
}