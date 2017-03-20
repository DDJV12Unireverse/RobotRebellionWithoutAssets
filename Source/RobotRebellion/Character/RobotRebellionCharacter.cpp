  // Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RobotRebellion.h"
#include "RobotRebellionCharacter.h"

#include "PlayableCharacter.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

#include "../Gameplay/Weapon/WeaponBase.h"
#include "../Gameplay/Weapon/WeaponInventory.h"

#include "../Gameplay/Alteration/StunAlteration.h"
#include "../Gameplay/Alteration/InvisibilityAlteration.h"

#include "../UI/TextBillboardComponent.h"
#include "../UI/LivingTextRenderComponent.h"

#include "../Tool/UtilitaryMacros.h"
#include "../Tool/UtilitaryFunctionLibrary.h"

#include "../Global/GameInstaller.h"


ARobotRebellionCharacter::ARobotRebellionCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    m_attribute = CreateDefaultSubobject<UAttributes>(TEXT("Attributes"));

    m_alterationController = CreateDefaultSubobject<UAlterationController>(TEXT("AlterationController"));
}

void ARobotRebellionCharacter::BeginPlay()
{
    Super::BeginPlay();

    this->m_timedDestroyDelegate = &ARobotRebellionCharacter::noDestroyForNow;
    this->m_disableBeforeDestroyDelegate = &ARobotRebellionCharacter::disablingEverything;
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

    (this->*m_timedDestroyDelegate)(deltaTime);
}

void ARobotRebellionCharacter::disablingEverything()
{
    this->bHidden = true;

    if (Controller)
    {
        Controller->UnPossess();
    }

    this->SetActorEnableCollision(false);

    this->UnregisterAllComponents();
    
    GetCapsuleComponent()->DestroyComponent();

    this->m_disableBeforeDestroyDelegate = &ARobotRebellionCharacter::endDisabling;
}

void ARobotRebellionCharacter::startTimedDestroy() USE_NOEXCEPT
{
    this->m_timedDestroyDelegate = &ARobotRebellionCharacter::destroyNow;
}

void ARobotRebellionCharacter::destroyNow(float deltaTime)
{
    (this->*m_disableBeforeDestroyDelegate)();


    //All conditions are met for destroying
    //To add a condition for destroying, add it to this if.
    //The destruction will occur when all conditions will be met
    if (m_textBillboardInstance->nothingToRender())
    {
        //We have made everything important before destroying. Now we can destroy safely.
        this->m_timedDestroyDelegate = &ARobotRebellionCharacter::noDestroyForNow;

        if (Role >= ROLE_Authority)
        {
            netMultiKill();
        }
        else if (!this->IsPendingKillOrUnreachable())
        {
            this->Destroy();
        }
    }
}

void ARobotRebellionCharacter::netMultiKill_Implementation()
{
    if (!this->IsPendingKillOrUnreachable())
    {
        this->ConditionalBeginDestroy();
    }
}

bool ARobotRebellionCharacter::netMultiKill_Validate()
{
    return true;
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

void ARobotRebellionCharacter::inflictStun()
{
    if(!this->isImmortal())
    {
        UStunAlteration* stunAlteration;

        if(UUtilitaryFunctionLibrary::createObjectFromDefaultWithoutAttach<UStunAlteration>(
            &stunAlteration,
            *GameAlterationInstaller::getInstance().getAlterationDefault<UStunAlteration>()
            ))
        {
            m_alterationController->addAlteration(stunAlteration);
        }
    }
}

void ARobotRebellionCharacter::inflictStun(float duration)
{
    if(!this->isImmortal())
    {
        UStunAlteration* stunAlteration;

        if(UUtilitaryFunctionLibrary::createObjectFromDefaultWithoutAttach<UStunAlteration>(
            &stunAlteration,
            *GameAlterationInstaller::getInstance().getAlterationDefault<UStunAlteration>()
            ))
        {
            stunAlteration->m_lifeTime = duration;
            m_alterationController->addAlteration(stunAlteration);
        }
    }
}

void ARobotRebellionCharacter::inflictInvisibility()
{
    if (!this->isImmortal())
    {
        UInvisibilityAlteration* invisibilityAlteration;
        if (UUtilitaryFunctionLibrary::createObjectFromDefaultWithoutAttach<UInvisibilityAlteration>(
            &invisibilityAlteration,
            *GameAlterationInstaller::getInstance().getAlterationDefault<UInvisibilityAlteration>()
        ))
        {
            m_alterationController->addAlteration(invisibilityAlteration);
        }
    }
}


void ARobotRebellionCharacter::setInvisible(bool isInvisible)
{
    UMeshComponent* characterMesh = FindComponentByClass<UMeshComponent>();
    if (characterMesh)
    {
        characterMesh->SetVisibility(!isInvisible);
    }

    if (Role >= ROLE_Authority)
    {
        multiSetInvisible(isInvisible);
    }
}

GENERATE_IMPLEMENTATION_METHOD_AND_DEFAULT_VALIDATION_METHOD(ARobotRebellionCharacter, multiSetInvisible, bool isInvisible)
{
    UMeshComponent* characterMesh = FindComponentByClass<UMeshComponent>();
    if (characterMesh)
    {
        characterMesh->SetVisibility(!isInvisible);
    }
}
