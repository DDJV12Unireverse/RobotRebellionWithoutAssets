// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ShortRangeWeapon.h"
#include "RobotRebellionCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "UObjectGlobals.h"
#include "GlobalDamageMethod.h"
#include "Damage.h"

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
    PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, "ShortAtt");
    bool alreadyHit = false;

    //Sphere for short range collision
    FVector MultiSphereStart = user->GetActorLocation() + FVector(0.f, 0.f, -m_weaponVerticallyRange) + m_weaponForwardRange*user->GetActorForwardVector();
    FVector MultiSphereEnd = MultiSphereStart + FVector(0.f, 0.f, 2.f* m_weaponVerticallyRange);
    
    //Considered Actors
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    
    //Ignored actors, only user
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(user);
    
    //Result


    TArray<FHitResult> OutHits;
    bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(user->GetWorld(), MultiSphereStart, MultiSphereEnd, m_weaponForwardRange*user->GetActorForwardVector().Size(), ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, 
        OutHits, 
        true);

    
    if (Result)
    {
        for (int32 noEnnemy = 0; noEnnemy < OutHits.Num(); ++noEnnemy)
        {
            FHitResult hit = OutHits[noEnnemy];

            ARobotRebellionCharacter* ennemy = static_cast<ARobotRebellionCharacter*>(hit.GetActor());
            if (!alreadyHit)
            {
                Damage damage{ static_cast<ARobotRebellionCharacter*>(m_owner), ennemy };
                ennemy->inflictDamage(damage(&UGlobalDamageMethod::normalHitWithWeaponComputed, 7.f));
                alreadyHit = true;
            }
        }
    }
}

