// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RobotRebellion.h"
#include "RobotRebellionCharacter.h"

#include "PlayableCharacter.h"

#include "Gameplay/Weapon/WeaponBase.h"
#include "Gameplay/Weapon/WeaponInventory.h"

#include "Gameplay/Alteration/StunAlteration.h"
#include "Gameplay/Alteration/InvisibilityAlteration.h"
#include "Gameplay/Alteration/ShieldAlteration.h"

#include "UI/TextBillboardComponent.h"
#include "UI/LivingTextRenderComponent.h"

#include "Tool/UtilitaryMacros.h"
#include "Tool/UtilitaryFunctionLibrary.h"

#include "Global/GameInstaller.h"

#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Global/WorldInstanceEntity.h"
#include "WidgetComponent.h"
#include "UI/LifeBarWidget.h"
#include "Global/EntityDataSingleton.h"


ARobotRebellionCharacter::ARobotRebellionCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    m_attribute = CreateDefaultSubobject<UAttributes>(TEXT("Attributes"));

    m_alterationController = CreateDefaultSubobject<UAlterationController>(TEXT("AlterationController"));

    m_isInCombat = false;


    m_isShieldAnimated = true;
}

void ARobotRebellionCharacter::BeginPlay()
{
    Super::BeginPlay();

    this->m_timedDestroyDelegate = &ARobotRebellionCharacter::noDestroyForNow;
    this->m_disableBeforeDestroyDelegate = &ARobotRebellionCharacter::disablingEverything;
    m_isRestoreManaParticleSpawned = false;
    m_isReviveParticleSpawned = false;
    m_isShieldParticleSpawned = false;

    m_tickCount = 0.f;
    m_burningBonesCount = 0;

    TArray<AActor*> entity;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldInstanceEntity::StaticClass(), entity);
    if(entity.Num() > 0)
    {
        m_worldEntity = Cast<AWorldInstanceEntity>(entity[0]);
    }

    m_bonesToUpdate = 0;
    m_bonesSet = 5;
    int32 bonesCount = GetMesh()->GetNumBones();
    m_burningBones.Reserve(bonesCount);
    m_fireEffects.Reserve(bonesCount);
    m_effectTimer.Reserve(bonesCount);

    m_decelerationCoeff = m_accelerationCoeff / 2.f;
    m_maxVelocity = m_maxWalkVelocity;

    m_healthBar = Cast<UWidgetComponent>(GetComponentByClass(UWidgetComponent::StaticClass()));
    if(m_healthBar)
    {
        ULifeBarWidget* widget = Cast<ULifeBarWidget>(m_healthBar->GetUserWidgetObject());
        if(widget)
        {
            widget->setOwner(this);
        }
    }
    m_location = ELocation::OUTSIDE;
}

void ARobotRebellionCharacter::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    if(m_textBillboardInstance)
    {
        m_textBillboardInstance->update(deltaTime);
    }
    else
    {
        this->createTextBillboard();
    }

    (this->*m_timedDestroyDelegate)(deltaTime);

    if(m_isRestoreManaParticleSpawned)
    {
        m_restoreManaEffectTimer += deltaTime;
        if(m_restoreManaEffectTimer >= m_restoreManaEffectDuration)
        {
            unspawnManaParticle();
        }
    }
    if(m_isReviveParticleSpawned)
    {
        m_reviveEffectTimer += deltaTime;
        if(m_reviveEffectTimer >= m_reviveEffectDuration)
        {
            unspawnReviveParticle();
        }
    }


    if(m_healthBar)
    {
        //Orient lifeBar for player camera
        APlayableCharacter* charac = Cast<APlayableCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
        if(charac)
        {
            UCameraComponent* camera = charac->GetFollowCamera();
            FRotator camRot = camera->GetComponentRotation();
            m_healthBar->SetWorldRotation(FRotator(-camRot.Pitch, camRot.Yaw + 180.f, camRot.Roll));
            m_healthBar->SetRelativeLocation(FVector(0.f, 0.f, charac->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 50.f));

        }
    }

    if(this->isBurning())
    {
        m_tickCount += deltaTime;
        if(m_tickCount >= 1.33f)
        {
            //GEngine->AddOnScreenDebugMessage(0 + 1, 10, FColor::Blue, FString::Printf(TEXT("size: %i"), m_burningBones.Num()));
            UpdateBurnEffect(m_tickCount);
            m_tickCount = 0.f;
        }
    }
}

void ARobotRebellionCharacter::disablingEverything()
{
    this->bHidden = true;

    if(Controller)
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
    if(m_textBillboardInstance->nothingToRender())
    {
        //We have made everything important before destroying. Now we can destroy safely.
        this->m_timedDestroyDelegate = &ARobotRebellionCharacter::noDestroyForNow;

        if(Role >= ROLE_Authority)
        {
            netMultiKill();
        }
        else if(!this->IsPendingKillOrUnreachable())
        {
            this->Destroy();
        }
    }
}

void ARobotRebellionCharacter::netMultiKill_Implementation()
{
    if(!this->IsPendingKillOrUnreachable())
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
    DOREPLIFETIME(ARobotRebellionCharacter, m_isInCombat);
    //DOREPLIFETIME(ARobotRebellionCharacter, m_burningBonesCount);
}

bool ARobotRebellionCharacter::hasDoubleWeapon() const USE_NOEXCEPT
{
    return m_weaponInventory->m_hasDoubleWeapon;
}

UWeaponBase* ARobotRebellionCharacter::getCurrentEquippedWeapon() const USE_NOEXCEPT
{
    return m_weaponInventory->getCurrentWeapon();
}

const UWeaponBase* ARobotRebellionCharacter::getMainWeapon() const USE_NOEXCEPT
{
    return m_weaponInventory->getMainWeapon();
}

const UWeaponBase* ARobotRebellionCharacter::getSecondaryWeapon() const USE_NOEXCEPT
{
    return m_weaponInventory->getSecondaryWeapon();
}

void ARobotRebellionCharacter::cppOnDeath()
{}

void ARobotRebellionCharacter::onDeath()
{
    if(Role == ROLE_Authority)
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

    if(Role >= ROLE_Authority)
    {
        netMultidisplayAnimatedIntegerValue(valueToDisplay, color, mode);
    }
}

void ARobotRebellionCharacter::displayAnimatedText(const FString& textToDisplay, const FColor& color, ELivingTextAnimMode mode)
{
    m_textBillboardInstance->beginDisplayingText(this->GetActorLocation(), textToDisplay, color, mode);

    if(Role >= ROLE_Authority)
    {
        netMultidisplayAnimatedText(textToDisplay, color, mode);
    }
}

void ARobotRebellionCharacter::netMultidisplayAnimatedIntegerValue_Implementation(int32 valueToDisplay, const FColor& color, ELivingTextAnimMode mode)
{
    if(m_textBillboardInstance)
    {
        m_textBillboardInstance->beginDisplayingInteger(this->GetActorLocation(), valueToDisplay, color, mode);
    }
}

void ARobotRebellionCharacter::netMultidisplayAnimatedText_Implementation(const FString& textToDisplay, const FColor& color, ELivingTextAnimMode mode)
{
    if(m_textBillboardInstance)
    {
        m_textBillboardInstance->beginDisplayingText(this->GetActorLocation(), textToDisplay, color, mode);
    }
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
    APlayableCharacter* charac = Cast<APlayableCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if(charac)
    {
        this->createTextBillboardWithThisCamera(charac->FollowCamera);
    }
}

void ARobotRebellionCharacter::createTextBillboardWithThisCamera(UCameraComponent* camera)
{
    if(UUtilitaryFunctionLibrary::createObjectFromDefault<UTextBillboardComponent>(&m_textBillboardInstance, m_textBillboardDefault, camera, RF_Dynamic))
    {
        setBillboardInstanceNewCamera(camera);
        m_textBillboardInstance->Activate();

        m_textBillboardInstance->RegisterComponent();
    }
}

void ARobotRebellionCharacter::setBillboardInstanceNewCamera(UCameraComponent* camera)
{
    if(NULL != m_textBillboardInstance)
    {
        m_textBillboardInstance->AttachToComponent(camera, FAttachmentTransformRules::KeepRelativeTransform);
        m_textBillboardInstance->SetRelativeTransform({});
        camera->UpdateChildTransforms();
    }
}

void ARobotRebellionCharacter::inflictStun()
{
    if(Role >= ROLE_Authority && !this->isImmortal())
    {
        this->internalInflictAlteration<UStunAlteration>(
            [](UStunAlteration* stunAlteration) {});
    }
}

void ARobotRebellionCharacter::inflictStun(float duration)
{
    if(Role >= ROLE_Authority && !this->isImmortal())
    {
        this->internalInflictAlteration<UStunAlteration>(
            [duration](UStunAlteration* stunAlteration) {
            stunAlteration->m_lifeTime = duration;
        });
    }
}

void ARobotRebellionCharacter::inflictInvisibility()
{
    if(Role >= ROLE_Authority)
    {
        this->internalInflictAlteration<UInvisibilityAlteration>([](UInvisibilityAlteration* invisibleAlteration) {});
    }
}

void ARobotRebellionCharacter::addShield(float amount, float duration)
{
    if(Role >= ROLE_Authority)
    {
        this->internalInflictAlteration<UShieldAlteration>(
            [amount, duration](UShieldAlteration* shieldAlteration) {
            shieldAlteration->m_lifeTime = duration;
            shieldAlteration->m_amount = amount;
        });
    }
}


void ARobotRebellionCharacter::setInvisible(bool isInvisible)
{
    updateInvisibilityMat(isInvisible);

    m_isInvisible = isInvisible;

    if(Role >= ROLE_Authority)
    {
        multiSetInvisible(isInvisible);
    }
}

void ARobotRebellionCharacter::updateInvisibilityMat_Implementation(bool isVisible)
{
    // does nothing
}

bool ARobotRebellionCharacter::isVisible() const
{
    return this->m_alterationController->findByID(IdentifiableObject<UInvisibilityAlteration>::ID.m_value) == nullptr;
}

void ARobotRebellionCharacter::inflictDamage(float damage, ELivingTextAnimMode animType, const FColor& damageColor)
{
    m_attribute->inflictDamage(damage);
    displayAnimatedIntegerValue(damage, damageColor, animType);

    if(isDead())
    {
        onDeath();
    }
}

void ARobotRebellionCharacter::restoreHealth(float value, ELivingTextAnimMode animType)
{
    m_attribute->restoreHealth(value);
    displayAnimatedIntegerValue(value, FColor::Green, animType);
}

void ARobotRebellionCharacter::restoreMana(float value, ELivingTextAnimMode animType)
{
    m_attribute->restoreMana(value);
    displayAnimatedIntegerValue(value, FColor::Blue, animType);
}

////RESTORE MANA EFFECT

void ARobotRebellionCharacter::spawnManaParticle()
{

    if(!m_restoreManaParticleSystem)
    {
        m_restoreManaParticleSystem = UGameplayStatics::SpawnEmitterAttached(m_restoreManaParticuleEffect, RootComponent, NAME_None,
                                                                             GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                                             GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
    }
    m_restoreManaParticleSystem->ActivateSystem(true);
    m_isRestoreManaParticleSpawned = true;
    if(Role >= ROLE_Authority)
    {
        multiSpawnManaParticle();
    }
}

void ARobotRebellionCharacter::unspawnManaParticle()
{
    m_restoreManaParticleSystem->DeactivateSystem();
    m_isRestoreManaParticleSpawned = false;
    if(Role >= ROLE_Authority)
    {
        m_restoreManaEffectTimer = 0.f;
        multiUnspawnManaParticle();
    }
}


void ARobotRebellionCharacter::multiSpawnManaParticle_Implementation()
{
    if(!m_restoreManaParticleSystem)
    {
        m_restoreManaParticleSystem = UGameplayStatics::SpawnEmitterAttached(m_restoreManaParticuleEffect, RootComponent, NAME_None,
                                                                             GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                                             GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
    }
    m_restoreManaParticleSystem->ActivateSystem(true);

    m_isRestoreManaParticleSpawned = true;
}

bool ARobotRebellionCharacter::multiSpawnManaParticle_Validate()
{
    return true;
}

void ARobotRebellionCharacter::multiUnspawnManaParticle_Implementation()
{
    m_restoreManaParticleSystem->DeactivateSystem();
    m_isRestoreManaParticleSpawned = false;
    m_restoreManaEffectTimer = 0.f;
}

bool ARobotRebellionCharacter::multiUnspawnManaParticle_Validate()
{
    return true;
}


////REVIVE EFFECT

void ARobotRebellionCharacter::spawnReviveParticle()
{

    if(!m_reviveParticleSystem)
    {
        m_reviveParticleSystem = UGameplayStatics::SpawnEmitterAttached(m_reviveParticuleEffect, RootComponent, NAME_None,
                                                                        GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                                        GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
    }
    m_reviveParticleSystem->ActivateSystem(true);
    m_isReviveParticleSpawned = true;
    if(Role >= ROLE_Authority)
    {
        multiSpawnReviveParticle();
    }
}

void ARobotRebellionCharacter::unspawnReviveParticle()
{
    m_reviveParticleSystem->DeactivateSystem();
    m_isReviveParticleSpawned = false;
    if(Role >= ROLE_Authority)
    {
        m_reviveEffectTimer = 0.f;
        multiUnspawnReviveParticle();
    }
}


void ARobotRebellionCharacter::multiSpawnReviveParticle_Implementation()
{
    if(!m_reviveParticleSystem)
    {
        m_reviveParticleSystem = UGameplayStatics::SpawnEmitterAttached(m_reviveParticuleEffect, RootComponent, NAME_None,
                                                                        GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                                        GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
    }
    m_reviveParticleSystem->ActivateSystem(true);

    m_isReviveParticleSpawned = true;
}

bool ARobotRebellionCharacter::multiSpawnReviveParticle_Validate()
{
    return true;
}

void ARobotRebellionCharacter::multiUnspawnReviveParticle_Implementation()
{
    m_reviveParticleSystem->DeactivateSystem();
    m_isReviveParticleSpawned = false;
    m_reviveEffectTimer = 0.f;
}

bool ARobotRebellionCharacter::multiUnspawnReviveParticle_Validate()
{
    return true;
}



GENERATE_IMPLEMENTATION_METHOD_AND_DEFAULT_VALIDATION_METHOD(ARobotRebellionCharacter, multiSetInvisible, bool isInvisible)
{
    updateInvisibilityMat(isInvisible);

    m_isInvisible = isInvisible;
}

UTextBillboardComponent* ARobotRebellionCharacter::getBillboardComponent()
{
    return m_textBillboardInstance;
}

////SHIELD EFFECT

void ARobotRebellionCharacter::spawnShieldParticle()
{

    if(!m_isShieldParticleSpawned)
    {

        if(m_isShieldAnimated)
        {
            m_shieldParticleSystem =
                UGameplayStatics::SpawnEmitterAttached(m_shieldParticuleEffect, RootComponent, NAME_None,
                                                       GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                       GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
        }
        else
        {
            m_shieldParticleSystem =
                UGameplayStatics::SpawnEmitterAttached(m_shieldParticuleEffectUnanimated, RootComponent, NAME_None,
                                                       GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                       GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
        }
    }

    // Test if shield animation has changed in option
    if(m_isShieldAnimated != m_worldEntity->isShieldAnimated())
    {
        // Destroye old particle emitter and build a new one
        m_shieldParticleSystem->DestroyComponent();
        m_isShieldAnimated = m_worldEntity->isShieldAnimated();
        if(m_isShieldAnimated)
        {
            m_shieldParticleSystem =
                UGameplayStatics::SpawnEmitterAttached(m_shieldParticuleEffect, RootComponent, NAME_None,
                                                       GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                       GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
        }
        else
        {
            m_shieldParticleSystem =
                UGameplayStatics::SpawnEmitterAttached(m_shieldParticuleEffectUnanimated, RootComponent, NAME_None,
                                                       GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                       GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
        }

    }
    m_shieldParticleSystem->ActivateSystem(true);
    m_isShieldParticleSpawned = true;
    if(Role >= ROLE_Authority)
    {
        multiSpawnShieldParticle();
    }
}

void ARobotRebellionCharacter::unspawnShieldParticle()
{
    m_shieldParticleSystem->DeactivateSystem();
    m_isShieldParticleSpawned = false;
    if(Role >= ROLE_Authority)
    {
        multiUnspawnShieldParticle();
    }
}


void ARobotRebellionCharacter::multiSpawnShieldParticle_Implementation()
{
    if(!m_isShieldParticleSpawned)
    {

        if(m_isShieldAnimated)
        {
            m_shieldParticleSystem =
                UGameplayStatics::SpawnEmitterAttached(m_shieldParticuleEffect, RootComponent, NAME_None,
                                                       GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                       GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
        }
        else
        {
            m_shieldParticleSystem =
                UGameplayStatics::SpawnEmitterAttached(m_shieldParticuleEffectUnanimated, RootComponent, NAME_None,
                                                       GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                       GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
        }
    }

    // Test if shield animation has changed in option
    if( m_isShieldAnimated != m_worldEntity->isShieldAnimated())
    {
        // Destroye old particle emitter and build a new one
        m_shieldParticleSystem->DestroyComponent();
        m_isShieldAnimated = m_worldEntity->isShieldAnimated();
        if(m_isShieldAnimated)
        {
            m_shieldParticleSystem =
                UGameplayStatics::SpawnEmitterAttached(m_shieldParticuleEffect, RootComponent, NAME_None,
                                                       GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                       GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
        }
        else
        {
            m_shieldParticleSystem =
                UGameplayStatics::SpawnEmitterAttached(m_shieldParticuleEffectUnanimated, RootComponent, NAME_None,
                                                       GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                       GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
        }
    }

    m_shieldParticleSystem->ActivateSystem(true);

    m_isShieldParticleSpawned = true;
}

bool ARobotRebellionCharacter::multiSpawnShieldParticle_Validate()
{
    return true;
}

void ARobotRebellionCharacter::multiUnspawnShieldParticle_Implementation()
{
    m_shieldParticleSystem->DeactivateSystem();
    m_isShieldParticleSpawned = false;
}

bool ARobotRebellionCharacter::multiUnspawnShieldParticle_Validate()
{
    return true;
}


////Burn Effect



void ARobotRebellionCharacter::UpdateBurnEffect(float DeltaTime)
{
    int burningBonesNumber = m_burningBones.Num();
    int nbBones = GetMesh()->GetNumBones();
    TArray<FName> bonesToBurn;
    for(int noCurrentBone = 0; noCurrentBone < burningBonesNumber; ++noCurrentBone)
    {
        int32 currentBoneId = m_burningBones[noCurrentBone];
        FName currentBoneName = GetMesh()->GetBoneName(currentBoneId);

        //compute if effect must be deactivated on this bone
        m_effectTimer[m_fireEffects[noCurrentBone]] += DeltaTime;
        if(m_effectTimer[m_fireEffects[noCurrentBone]] >= 3.f && (m_fireEffects[noCurrentBone])->IsActive())
        {
            //m_fireEffects[noCurrentBone]->DestroyComponent();
            m_fireEffects[noCurrentBone]->Deactivate();
            --m_burningBonesCount;
            //GEngine->AddOnScreenDebugMessage(1, 10, FColor::Blue, FString::Printf(TEXT("number %i"), m_burningBonesCount));
            m_effectTimer[m_fireEffects[noCurrentBone]] = 0.f;
            continue;
        }


        // PARENT Bone
        FName parentName = GetMesh()->GetParentBone(currentBoneName);
        int32 parentid = GetMesh()->GetBoneIndex(parentName);
        int32 intIsPresent = m_burningBones.Find(parentid);

        if(intIsPresent == -1)
        {
            bonesToBurn.Emplace(parentName);
            continue;

            //CHILDRENBONE
            // Not very good performances, no other ways found (dont update every bone each time for better performances)
            for(int i = currentBoneId + 1 + m_bonesToUpdate; i < nbBones; i += m_bonesSet)
            {
                m_bonesToUpdate = (m_bonesToUpdate + 1) % m_bonesSet;
                int32 isChildPresent = m_burningBones.Find(i); //Check if children already on fire
                FName boneName = GetMesh()->GetBoneName(i);
                if(GetMesh()->BoneIsChildOf(boneName, currentBoneName) && isChildPresent == -1)
                {
                    bonesToBurn.Emplace(boneName);
                }
            }
        }
    }

    displayFireOnBoneArray(bonesToBurn);

    if(m_burningBonesCount <= 0)
    {
        cleanFireComp();
    }

}

void ARobotRebellionCharacter::displayFireOnBone(const FName& bone)
{
    internalDisplayFireOnBone(bone);

    if(Role >= ROLE_Authority)
    {
        multiDisplayFireOnBone(bone);
    }
}

void ARobotRebellionCharacter::multiDisplayFireOnBone_Implementation(const FName& bone)
{
    if(m_worldEntity->IsBurnEffectEnabled())
    {
        internalDisplayFireOnBone(bone);
    }
}


void ARobotRebellionCharacter::internalDisplayFireOnBone(const FName& bone)
{
    FVector boneLocation = GetMesh()->GetBoneLocation(bone);
    FTransform boneTransform = GetMesh()->GetBoneTransform(GetMesh()->GetBoneIndex(bone));

    UParticleSystemComponent* fireEffect = UGameplayStatics::SpawnEmitterAttached(m_fireEffect, GetRootComponent(),
                                                                                  NAME_None, boneLocation,
                                                                                  FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, false);
    if(fireEffect)
    {
        fireEffect->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
        m_burningBones.Emplace(GetMesh()->GetBoneIndex(bone));
        m_fireEffects.Emplace(fireEffect);
        m_effectTimer.Emplace(fireEffect, 0.f);
        ++m_burningBonesCount;
    }
}

void ARobotRebellionCharacter::displayFireOnBoneArray(const TArray<FName>& bones)
{

    internalDisplayFireOnBoneArray(bones);

    if(Role >= ROLE_Authority)
    {
        multiDisplayFireOnBoneArray(bones);
    }
}

void ARobotRebellionCharacter::internalDisplayFireOnBoneArray(const TArray<FName>& bones)
{
    int size = bones.Num();
    int max = (size >= 5 ? 5 : size); // limit size for performance
    for(int i = 0; i < max; ++i)
    {
        FName bone = bones[i];
        int32 boneLocationIndex = GetMesh()->GetBoneIndex(bone);
        FVector boneLocation = GetMesh()->GetBoneLocation(bone);

        UParticleSystemComponent* fireEffect = UGameplayStatics::SpawnEmitterAttached(m_fireEffect, GetRootComponent(),
                                                                                      NAME_None, boneLocation,
                                                                                      FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, false);
        if(fireEffect)
        {
            fireEffect->SetRelativeScale3D({0.4f, 0.4f, 0.4f});
            m_burningBones.Emplace(boneLocationIndex);
            m_fireEffects.Emplace(fireEffect);
            m_effectTimer.Emplace(fireEffect, 0.f);
            ++m_burningBonesCount;
        }
    }
}

void ARobotRebellionCharacter::multiDisplayFireOnBoneArray_Implementation(const TArray<FName>& bones)
{
    if(m_worldEntity->IsBurnEffectEnabled())
    {
        internalDisplayFireOnBoneArray(bones);
    }
}

void ARobotRebellionCharacter::internalSpawnFireEffect(FVector location)
{
    if(m_worldEntity->IsBurnEffectEnabled())
    {
        FName bone = GetMesh()->FindClosestBone(location);
        int32 boneIndex = GetMesh()->GetBoneIndex(bone);
        int32 intIsPresent = (m_burningBones.Find(boneIndex));
        if(intIsPresent == -1)
        {
            displayFireOnBone(bone);
        }
    }
}



void ARobotRebellionCharacter::spawnFireEffect(FVector location)
{
    if(!isBurning())
    {
        internalSpawnFireEffect(location);

        if(Role >= ROLE_Authority)
        {
            multiSpawnFireEffect(location);
        }
    }
}


void ARobotRebellionCharacter::multiSpawnFireEffect_Implementation(FVector location)
{
    internalSpawnFireEffect(location);
}

void ARobotRebellionCharacter::internalCleanFireComp()
{
    m_burningBones.Reset();
    m_fireEffects.Reset();
    m_effectTimer.Reset();
    m_burningBonesCount = 0;

}

void ARobotRebellionCharacter::cleanFireComp()
{
    internalCleanFireComp();

    if(Role >= ROLE_Authority)
    {
        multiCleanFireComp();
    }
    else
    {
        serverCleanFireComp();
    }


}

void ARobotRebellionCharacter::multiCleanFireComp_Implementation()
{
    internalCleanFireComp();
}

void ARobotRebellionCharacter::serverCleanFireComp_Implementation()
{
    cleanFireComp();
}
bool ARobotRebellionCharacter::serverCleanFireComp_Validate()
{
    return true;
}