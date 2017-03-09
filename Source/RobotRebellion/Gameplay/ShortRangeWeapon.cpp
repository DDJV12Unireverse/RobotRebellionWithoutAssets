// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ShortRangeWeapon.h"
#include "../Character/RobotRebellionCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "UObjectGlobals.h"
#include "../Global/GlobalDamageMethod.h"
#include "Damage.h"
#include "DamageCoefficientLogic.h"

/************************************************************************/
/*                  CONSTRUCTORS                                        */
/************************************************************************/

UShortRangeWeapon::UShortRangeWeapon():UWeaponBase()
{
    m_weaponForwardRange = 75.f;
    m_weaponVerticallyRange = 75.f;
}


/************************************************************************/
/*                  METHODS                                           */
/************************************************************************/
void UShortRangeWeapon::cppAttack(ARobotRebellionCharacter* user)
{
    if (canAttack())
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, "ShortAtt");
        bool alreadyHit = false;

        //Sphere for short range collision
        FVector MultiSphereStart = user->GetActorLocation() + FVector(0.f, 0.f, -m_weaponVerticallyRange) + m_weaponForwardRange*user->GetActorForwardVector();
        FVector MultiSphereEnd = MultiSphereStart + FVector(0.f, 0.f, 2.f * m_weaponVerticallyRange);

        //Considered Actors
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

        //Ignored actors, only user
        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(user);

        //Result

        TArray<FHitResult> OutHits;

        bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(
            user->GetWorld(),
            MultiSphereStart,
            MultiSphereEnd,
            m_weaponForwardRange * user->GetActorForwardVector().Size(),
            ObjectTypes,
            false,
            ActorsToIgnore,
            EDrawDebugTrace::None,
            OutHits,
            true
        );


        if (Result)
        {
            //CAN BE OPTIMIZED
            for (int32 noEnnemy = 0; noEnnemy < OutHits.Num(); ++noEnnemy)
            {
                FHitResult hit = OutHits[noEnnemy];

                ARobotRebellionCharacter* receiver = static_cast<ARobotRebellionCharacter*>(hit.GetActor());
                if (!alreadyHit)
                {
                    if (!receiver->isImmortal())
                    {

                        DamageCoefficientLogic coeff;

                        Damage damage{ static_cast<ARobotRebellionCharacter*>(m_owner), receiver };
                        Damage::DamageValue damageValue = damage(&UGlobalDamageMethod::normalHitWithWeaponComputed, coeff.getCoefficientValue());

                        receiver->inflictDamage(damageValue);
                        receiver->displayAnimatedIntegerValue(damageValue, FColor::Red, ELivingTextAnimMode::TEXT_ANIM_MOVING);


                        if (receiver->isDead())
                        {
                            receiver->onDeath();
                        }
                    }
                    else
                    {
                        receiver->displayAnimatedText("IMMORTAL OBJECT", FColor::Purple, ELivingTextAnimMode::TEXT_ANIM_NOT_MOVING);
                    }

                    alreadyHit = true;
                }
            }
        }

        reload();
    }
    else
    {
        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "Cannot attack! Let me breath!");
    }
}

FString UShortRangeWeapon::rangeToFString() const USE_NOEXCEPT
{
    return "Short Range weapon";
}