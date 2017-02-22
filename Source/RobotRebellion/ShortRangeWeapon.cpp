// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "ShortRangeWeapon.h"
#include "RobotRebellionCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "UObjectGlobals.h"

/************************************************************************/
/*                  CONSTRUCTOR                                           */
/************************************************************************/
UShortRangeWeapon::UShortRangeWeapon():UWeaponBase()
{
    //this->AttachChildren(UCapsuleComponent(50.f, 100.f)));
    m_attackZone= CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    m_attackZone->InitCapsuleSize(500.f, 1000.f);
    m_attackZone->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
    m_attackZone->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
    m_attackZone->bGenerateOverlapEvents = true;
    m_attackZone->OnComponentBeginOverlap.AddDynamic(this, &UShortRangeWeapon::OnBeginOverlap);
}

/************************************************************************/
/*                  UFUNCTION                                           */
/************************************************************************/

void UShortRangeWeapon::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, "Contact MotherFucker");

}

/************************************************************************/
/*                  METHODS                                           */
/************************************************************************/
void UShortRangeWeapon::cppAttack(ARobotRebellionCharacter* user)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, "ShortAtt");

    //Sphere for short range collision
    FVector MultiSphereStart = user->GetActorLocation() + 5.f*user->GetActorForwardVector();
    FVector MultiSphereEnd = MultiSphereStart + FVector(0.f, 0.f, 15.f);
    //Considered Actors
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    //Ignored actors, only user
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(user);
    //Result
    TArray<FHitResult> OutHits;
    bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(user->GetWorld(), MultiSphereStart, MultiSphereEnd, 5.0f*user->GetActorForwardVector().Size(), ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, // Le type de Debug
        OutHits, // Où seront stockés les résultats
        true);
   
    
}

