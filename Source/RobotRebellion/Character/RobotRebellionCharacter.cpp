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


ARobotRebellionCharacter::ARobotRebellionCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    m_attribute = CreateDefaultSubobject<UAttributes>(TEXT("Attributes"));

    m_alterationController = CreateDefaultSubobject<UAlterationController>(TEXT("AlterationController"));

    m_isInCombat = false;

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

    m_tickCount += deltaTime;
    if(isBurning() && m_tickCount >= 1.f)
    {
        GEngine->AddOnScreenDebugMessage(0 + 1, 10, FColor::Blue, FString::Printf(TEXT("size: %i"), m_burningBones.Num()));
        UpdateBurnEffect(m_tickCount);
        m_tickCount = 0.f;
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
}

UWeaponBase* ARobotRebellionCharacter::getCurrentEquippedWeapon() const USE_NOEXCEPT
{
    return m_weaponInventory->getCurrentWeapon();
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
    if(!this->isImmortal())
    {
        UInvisibilityAlteration* invisibilityAlteration;
        if(UUtilitaryFunctionLibrary::createObjectFromDefaultWithoutAttach<UInvisibilityAlteration>(
            &invisibilityAlteration,
            *GameAlterationInstaller::getInstance().getAlterationDefault<UInvisibilityAlteration>()
            ))
        {
            m_alterationController->addAlteration(invisibilityAlteration);
        }
    }
}

void ARobotRebellionCharacter::addShield(float amount, float duration)
{
    if(!this->isImmortal())
    {
        UShieldAlteration* shieldAlteration;
        if(UUtilitaryFunctionLibrary::createObjectFromDefaultWithoutAttach<UShieldAlteration>(
            &shieldAlteration,
            *GameAlterationInstaller::getInstance().getAlterationDefault<UShieldAlteration>()
            ))
        {
            shieldAlteration->m_lifeTime = duration;
            shieldAlteration->m_amount = amount;
            m_alterationController->addAlteration(shieldAlteration);
        }
    }
}


void ARobotRebellionCharacter::setInvisible(bool isInvisible)
{
    UMeshComponent* characterMesh = FindComponentByClass<UMeshComponent>();
    APlayableCharacter* player = Cast<APlayableCharacter>(this);
    if(player)
    {
        if(isInvisible)
        {
            characterMesh->SetVisibility(!isInvisible);
            player->m_fpsMesh->SetVisibility(!isInvisible);
        }
        else
        {
            UMeshComponent* mesh = player->getCurrentViewMesh();
            mesh->SetVisibility(!isInvisible);
        }
    }
    else
    {
        if(characterMesh)
        {
            characterMesh->SetVisibility(!isInvisible);
        }
    }

    m_isInvisible = isInvisible;

    if(Role >= ROLE_Authority)
    {
        multiSetInvisible(isInvisible);
    }
}

bool ARobotRebellionCharacter::isVisible()
{
    UMeshComponent* characterMesh = FindComponentByClass<UMeshComponent>();
    APlayableCharacter* player = Cast<APlayableCharacter>(this);
    if(player)
    {
        return player->getCurrentViewMesh()->IsVisible();
    }
    else
    {
        if(characterMesh)
        {
            return characterMesh->IsVisible();
        }
    }

    return false;
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
    UMeshComponent* characterMesh = FindComponentByClass<UMeshComponent>();
    APlayableCharacter* player = Cast<APlayableCharacter>(this);
    if(player)
    {
        if(isInvisible)
        {
            characterMesh->SetVisibility(!isInvisible);
            player->m_fpsMesh->SetVisibility(!isInvisible);
        }
        else
        {
            UMeshComponent* mesh = player->getCurrentViewMesh();
            mesh->SetVisibility(!isInvisible);
        }
    }
    else
    {
        if(characterMesh)
        {
            characterMesh->SetVisibility(!isInvisible);
        }
    }

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
        m_shieldParticleSystem = UGameplayStatics::SpawnEmitterAttached(m_shieldParticuleEffect, RootComponent, NAME_None,
                                                                        GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                                        GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
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
        m_shieldParticleSystem = UGameplayStatics::SpawnEmitterAttached(m_shieldParticuleEffect, RootComponent, NAME_None,
                                                                        GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
                                                                        GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
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
    // GEngine->AddOnScreenDebugMessage(0, 10, FColor::Green, FString::Printf(TEXT("update: %i"), m_tickCount));
    int burningBonesNumber = m_burningBones.Num();
    // TArray<int32> particlesToRemove;
    for(int noCurrentBone = 0; noCurrentBone < burningBonesNumber; ++noCurrentBone)
    {
        int32 currentBoneId = m_burningBones[noCurrentBone];
        FName currentBoneName = GetMesh()->GetBoneName(currentBoneId);
        //compute if effect must be deactivated on this bone
        m_effectTimer[m_fireEffects[noCurrentBone]] += DeltaTime;
        if(m_effectTimer[m_fireEffects[noCurrentBone]] >= 5.f)
        {
            //  m_burningBones.Remove(currentBoneId);
            m_fireEffects[noCurrentBone]->DestroyComponent();
            //m_fireEffects[noCurrentBone]->Deactivate();
            --m_burningBonesCount;
            GEngine->AddOnScreenDebugMessage(1, 10, FColor::Blue, FString::Printf(TEXT("number %i"), m_burningBonesCount));
            m_effectTimer[m_fireEffects[noCurrentBone]] = 0.f;
        }


        // PARENT Bone
        FName parentName = GetMesh()->GetParentBone(GetMesh()->GetBoneName(currentBoneId));
        int32 parentid = GetMesh()->GetBoneIndex(parentName);
        int32 intIsPresent = m_burningBones.Find(parentid);

        if(intIsPresent == -1 && parentName != FName("root"))
        {
            FString debugName;
            int nbBones = GetMesh()->GetNumBones();

            parentName.AppendString(debugName);

            // GEngine->AddOnScreenDebugMessage(noCurrentBone + 1, 10, FColor::Blue, debugName);
            //             if(parentName != FName("root"))
            //             {
            //             }
            displayFireOnBone(parentName);


            //CHILDRENBONE
            // Not very good performances, no other ways found
            for(int i = 0; i < GetMesh()->GetNumBones(); ++i)
            {
                int32 isChildPresent = m_burningBones.Find(i); //Check if children already on fire
                if(GetMesh()->BoneIsChildOf(GetMesh()->GetBoneName(i), currentBoneName) && isChildPresent == -1)
                {
                    displayFireOnBone(GetMesh()->GetBoneName(i));
                }
            }
        }

    }
    if(m_burningBonesCount <= 0)
    {
        cleanFireComp();
    }

}

void ARobotRebellionCharacter::displayFireOnBone(FName bone)
{
    if(bone != FName("None"))
    {
        FVector boneLocation = GetMesh()->GetBoneLocation(bone);
        //GEngine->AddOnScreenDebugMessage(GetMesh()->GetBoneIndex(bone), 5, FColor::Black, FString::Printf(TEXT("Bone %i"), GetMesh()->GetBoneIndex(bone)));
        FTransform boneTransform = GetMesh()->GetBoneTransform(GetMesh()->GetBoneIndex(bone));

        UParticleSystemComponent* fireEffect = UGameplayStatics::SpawnEmitterAttached(m_fireEffect, GetRootComponent(),
                                                                                      NAME_None, boneLocation,
                                                                                      FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, false);
        if(fireEffect)
        {
            fireEffect->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
            m_burningBones.Add(GetMesh()->GetBoneIndex(bone));
            // GEngine->AddOnScreenDebugMessage(0, 15, FColor::Cyan, FString::Printf(TEXT("size %i"), m_fireEffects.Num()));

            m_fireEffects.Add(fireEffect);
            m_effectTimer.Add(fireEffect, 0.f);
            ++m_burningBonesCount;
            GEngine->AddOnScreenDebugMessage(1, 10, FColor::Blue, FString::Printf(TEXT("number %i"), m_burningBonesCount));
        }
    }

    if(Role >= ROLE_Authority)
    {
        multiDisplayFireOnBone(bone);
    }
}

void ARobotRebellionCharacter::multiDisplayFireOnBone_Implementation(FName bone)
{
    if(bone != FName("None"))
    {
        FVector boneLocation = GetMesh()->GetBoneLocation(bone);
        //GEngine->AddOnScreenDebugMessage(GetMesh()->GetBoneIndex(bone), 5, FColor::Black, FString::Printf(TEXT("Bone %i"), GetMesh()->GetBoneIndex(bone)));
        FTransform boneTransform = GetMesh()->GetBoneTransform(GetMesh()->GetBoneIndex(bone));

        UParticleSystemComponent* fireEffect = UGameplayStatics::SpawnEmitterAttached(m_fireEffect, GetRootComponent(),
                                                                                      NAME_None, boneLocation,
                                                                                      FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, false);
        if(fireEffect)
        {
            fireEffect->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
            m_burningBones.Add(GetMesh()->GetBoneIndex(bone));
            // GEngine->AddOnScreenDebugMessage(0, 15, FColor::Cyan, FString::Printf(TEXT("size %i"), m_fireEffects.Num()));

            m_fireEffects.Add(fireEffect);
            m_effectTimer.Add(fireEffect, 0.f);
            ++m_burningBonesCount;
            GEngine->AddOnScreenDebugMessage(1, 10, FColor::Blue, FString::Printf(TEXT("number %i"), m_burningBonesCount));
        }
    }
}

void ARobotRebellionCharacter::spawnFireEffect(FVector location)
{
    if(!isBurning())
    {
        if(Role >= ROLE_Authority)
        {
            FName bone = GetMesh()->FindClosestBone(location);
            int32 boneIndex = GetMesh()->GetBoneIndex(bone);
            int32 intIsPresent = (m_burningBones.Find(boneIndex));
            FString debugName;
            bone.AppendString(debugName);
            GEngine->AddOnScreenDebugMessage(0, 5, FColor::Red, debugName);
            if(intIsPresent == -1)
            {
                displayFireOnBone(bone);
            }
            multiSpawnFireEffect(location);
        }
        else
        {
            serverSpawnFireEffect(location);
        }
    }
}

void ARobotRebellionCharacter::serverSpawnFireEffect_Implementation(FVector location)
{
    spawnFireEffect(location);
}

bool ARobotRebellionCharacter::serverSpawnFireEffect_Validate(FVector location)
{
    return true;
}

void ARobotRebellionCharacter::multiSpawnFireEffect_Implementation(FVector location)
{
    FName bone = GetMesh()->FindClosestBone(location);
    int32 boneIndex = GetMesh()->GetBoneIndex(bone);
    int32 intIsPresent = (m_burningBones.Find(boneIndex));
    FString debugName;
    bone.AppendString(debugName);
    GEngine->AddOnScreenDebugMessage(0, 5, FColor::Red, debugName);
    if(intIsPresent == -1)
    {
        displayFireOnBone(bone);
    }
}
void ARobotRebellionCharacter::cleanFireComp()
{

    m_burningBones.Empty();
    // m_burningBones.RemoveAll(true);
    m_fireEffects.Empty();
    GEngine->AddOnScreenDebugMessage(7, 10, FColor::Blue, FString::Printf(TEXT("size: %i"), m_burningBones.Num()));
    //m_fireEffects.RemoveAll(true);
    //m_fireEffects.Shrink()
    m_burningBonesCount = 0;
    TArray<UActorComponent*> fireComponents = GetComponentsByClass(UParticleSystemComponent::StaticClass());
    fireComponents.RemoveAll([&](UActorComponent* comp) {
        UParticleSystemComponent* systComp = Cast<UParticleSystemComponent>(comp);
        //Change for integration
        systComp->DestroyComponent();
        if(systComp)
        {
            return true;
        }
        return false;
    });

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
    m_burningBones.Empty();
    // m_burningBones.RemoveAll(true);
    m_fireEffects.Empty();
    GEngine->AddOnScreenDebugMessage(7, 10, FColor::Blue, FString::Printf(TEXT("size: %i"), m_burningBones.Num()));
    //m_fireEffects.RemoveAll(true);
    //m_fireEffects.Shrink()
    m_burningBonesCount = 0;
    TArray<UActorComponent*> fireComponents = GetComponentsByClass(UParticleSystemComponent::StaticClass());
    fireComponents.RemoveAll([&](UActorComponent* comp) {
        UParticleSystemComponent* systComp = Cast<UParticleSystemComponent>(comp);
        //systComp->Get
        if(systComp)
        {
            return true;
        }
        return false;
    });
}

void ARobotRebellionCharacter::serverCleanFireComp_Implementation()
{
    cleanFireComp();
}
bool ARobotRebellionCharacter::serverCleanFireComp_Validate()
{
    return true;
}