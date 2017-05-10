// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "NonPlayableCharacter.h"

#include "UI/TextBillboardComponent.h"
#include "IA/Controller/CustomAIControllerBase.h"


ANonPlayableCharacter::ANonPlayableCharacter() : ARobotRebellionCharacter()
{
    // fill it
    m_lootTable = CreateDefaultSubobject<ULootTable>(TEXT("LootTable"));
    GetCharacterMovement()->bOrientRotationToMovement = false;
}


void ANonPlayableCharacter::cppOnDeath()
{
    dropLoot();
    this->m_alterationController->removeAllAlteration();
    this->cleanFireComp();
    ACustomAIControllerBase* controller = Cast<ACustomAIControllerBase>(this->GetController());
    if (controller)
    {
        controller->setTarget(nullptr);
    }

    this->startTimedDestroy();
}


void ANonPlayableCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ANonPlayableCharacter, m_isCrouch, COND_SkipOwner);
}

void ANonPlayableCharacter::dropLoot()
{
    if (Role == ROLE_Authority)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Black, "Drop!");
        m_lootTable->dropItem(GetActorLocation());
    }
    else
    {
        serverDropLoot();
    }
        
}
void ANonPlayableCharacter::serverDropLoot_Implementation()
{
    dropLoot();
}

bool ANonPlayableCharacter::serverDropLoot_Validate()
{
    return true;
}

FVector ANonPlayableCharacter::aim(const FVector& directionToShoot) const
{                                                                                                        
    FVector result = directionToShoot;

    ACustomAIControllerBase* controller = Cast<ACustomAIControllerBase>(Controller);

    //No ACustomAIControllerBase. Please attach one in BP if you want to fire
    check(controller);

    controller->aim(result);

    return result;
}

void ANonPlayableCharacter::spawnEffect()
{
    UGameplayStatics::SpawnEmitterAtLocation(
        GetWorld(),
        m_spawnParticleSystem,
        GetActorLocation(),
        GetActorRotation(),
        true
    );

    if(RootComponent->GetOwnerRole() >= ROLE_Authority)
    {
        multiSpawnEffect();
    }
}

void ANonPlayableCharacter::multiSpawnEffect_Implementation()
{
    UGameplayStatics::SpawnEmitterAtLocation(
        GetWorld(),
        m_spawnParticleSystem,
        GetActorLocation(),
        GetActorRotation(),
        true
    );
}

bool ANonPlayableCharacter::multiSpawnEffect_Validate()
{
    return true;
}

void ANonPlayableCharacter::goAway(const FVector& fromWhere, float delta)
{
    ACustomAIControllerBase* controller = Cast<ACustomAIControllerBase>(Controller);

    if (controller)
    {
        FVector actorLocation = this->GetActorLocation();
        FVector fireDirection = actorLocation - fromWhere;
        fireDirection.Normalize();

        FVector toMove = FVector::CrossProduct(this->GetActorUpVector(), fireDirection);
        toMove.Normalize();

        toMove *= delta;

        controller->StopMovement(); //stop the move it did before
        controller->MoveToLocation(actorLocation + toMove);
    }
}
