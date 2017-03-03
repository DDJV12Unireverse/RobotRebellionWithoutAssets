// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RobotRebellion.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "RobotRebellionCharacter.h"

#include "WeaponBase.h"
#include "WeaponInventory.h"

#include "TextBillboardComponent.h"
#include "LivingTextRenderComponent.h"
#include "PlayableCharacter.h"


#include "UtilitaryMacros.h"
#include "UtilitaryFunctionLibrary.h"



ARobotRebellionCharacter::ARobotRebellionCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    m_attribute = CreateDefaultSubobject<UAttributes>(TEXT("Attributes"));
}

void ARobotRebellionCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ARobotRebellionCharacter::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    if (m_textBillboardInstance)
    {
        m_textBillboardInstance->update(deltaTime);
    }
    else
    {
        this->createTextBillboard();
    }
}

///// SERVER
void ARobotRebellionCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ARobotRebellionCharacter, m_attribute);
}

UWeaponBase* ARobotRebellionCharacter::getCurrentEquippedWeapon() const USE_NOEXCEPT
{
    return m_weaponInventory->getCurrentWeapon();
}

void ARobotRebellionCharacter::cppOnDeath()
{}

void ARobotRebellionCharacter::onDeath()
{
    if (Role == ROLE_Authority)
    {
        clientOnDeath();
        //return;
    }
        
    this->cppOnDeath();
}

void ARobotRebellionCharacter::clientOnDeath_Implementation()
{
    this->cppOnDeath();
}

bool ARobotRebellionCharacter::clientOnDeath_Validate()
{
    return true;
}

void ARobotRebellionCharacter::cppOnRevive()
{}

void ARobotRebellionCharacter::displayAnimatedIntegerValue(int32 valueToDisplay, const FColor& color, ELivingTextAnimMode mode)
{
    m_textBillboardInstance->beginDisplayingInteger(this->GetActorLocation(), valueToDisplay, color, mode);

    if (Role >= ROLE_Authority)
    {
        netMultidisplayAnimatedIntegerValue(valueToDisplay, color, mode);
    }
}

void ARobotRebellionCharacter::displayAnimatedText(const FString& textToDisplay, const FColor& color, ELivingTextAnimMode mode)
{
    m_textBillboardInstance->beginDisplayingText(this->GetActorLocation(), textToDisplay, color, mode);

    if (Role >= ROLE_Authority)
    {
        netMultidisplayAnimatedText(textToDisplay, color, mode);
    }
}

void ARobotRebellionCharacter::netMultidisplayAnimatedIntegerValue_Implementation(int32 valueToDisplay, const FColor& color, ELivingTextAnimMode mode)
{
    m_textBillboardInstance->beginDisplayingInteger(this->GetActorLocation(), valueToDisplay, color, mode);
}

void ARobotRebellionCharacter::netMultidisplayAnimatedText_Implementation(const FString& textToDisplay, const FColor& color, ELivingTextAnimMode mode)
{
    m_textBillboardInstance->beginDisplayingText(this->GetActorLocation(), textToDisplay, color, mode);
}

bool ARobotRebellionCharacter::netMultidisplayAnimatedIntegerValue_Validate(int32 valueToDisplay, const FColor& color, ELivingTextAnimMode mode)
{
    return true;
}

bool ARobotRebellionCharacter::netMultidisplayAnimatedText_Validate(const FString& textToDisplay, const FColor& color, ELivingTextAnimMode mode)
{
    return true;
}

void ARobotRebellionCharacter::createTextBillboard()
{
    this->createTextBillboardWithThisCamera(Cast<APlayableCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->FollowCamera);
}

void ARobotRebellionCharacter::createTextBillboardWithThisCamera(UCameraComponent* camera)
{
    if (UUtilitaryFunctionLibrary::createObjectFromDefault<UTextBillboardComponent>(&m_textBillboardInstance, m_textBillboardDefault, camera, RF_Dynamic))
    {
        m_textBillboardInstance->AttachToComponent(camera, FAttachmentTransformRules::KeepRelativeTransform);
        m_textBillboardInstance->SetRelativeTransform({});
        camera->UpdateChildTransforms();
        m_textBillboardInstance->Activate();

        m_textBillboardInstance->RegisterComponent();
    }
}
