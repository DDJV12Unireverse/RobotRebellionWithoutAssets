// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ShortRangeWeapon.h"

#include "Character/RobotRebellionCharacter.h"
#include "../../Global/GlobalDamageMethod.h"

#include "../Damage/Damage.h"
#include "../Damage/DamageCoefficientLogic.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "UObjectGlobals.h"

/************************************************************************/
/*                  CONSTRUCTORS                                        */
/************************************************************************/

UShortRangeWeapon::UShortRangeWeapon() :UWeaponBase()
{
    m_weaponForwardRange = 75.f;
    m_weaponVerticallyRange = 75.f;
}


/************************************************************************/
/*                  METHODS                                           */
/************************************************************************/
void UShortRangeWeapon::cppAttack(ARobotRebellionCharacter* user)
{
    if(canAttack())
    {
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

        if(UKismetSystemLibrary::SphereTraceMultiForObjects(
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
        ))
        {
            ARobotRebellionCharacter** exReceiver = nullptr;
            int32 outCount = OutHits.Num();

            if(outCount <= 0)
            {
                playSound(m_missSound, user);
            }
            else
            {
                for(int32 noEnnemy = 0; noEnnemy < outCount; ++noEnnemy)
                {
                    FHitResult hit = OutHits[noEnnemy];

                    ARobotRebellionCharacter* receiver = Cast<ARobotRebellionCharacter>(hit.GetActor());
                    if(receiver && exReceiver != &receiver && !receiver->isDead())
                    {
                        this->inflictDamageLogic(receiver, hit);

                        exReceiver = &receiver;
                    }
                }

                playSound(m_hitSound, user);
            }
        }
        else
        {
            playSound(m_missSound, user);
        }

        reload();
    }
}

void UShortRangeWeapon::cppAttack(ARobotRebellionCharacter* user, ARobotRebellionCharacter* ennemy)
{
    cppAttack(user);
}

void UShortRangeWeapon::inflictDamageLogic(ARobotRebellionCharacter* receiver, const FHitResult& hit)
{
    if(!receiver->isImmortal())
    {
        DamageCoefficientLogic coeff;

        FVector ownerToReceiver = receiver->GetActorLocation() - m_owner->GetActorLocation();
        ownerToReceiver.Normalize();

        if(FVector::DotProduct(ownerToReceiver, receiver->GetActorForwardVector()) > 0.25f)
        {
            PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Yellow, "BackStab");
            coeff.backstab();
        }

        Damage damage{ Cast<ARobotRebellionCharacter>(m_owner), receiver };
        Damage::DamageValue damageValue = damage(&UGlobalDamageMethod::normalHitWithWeaponComputed, coeff.getCoefficientValue());

        receiver->inflictDamage(damageValue);
    }
}

FString UShortRangeWeapon::rangeToFString() const USE_NOEXCEPT
{
    return "Short Range weapon";
}

void UShortRangeWeapon::playSound_Implementation(USoundCue* sound, AActor* originator)
{
    if(sound && originator)
    {
        UGameplayStatics::SpawnSoundAttached(sound, originator->GetRootComponent());
    }
}