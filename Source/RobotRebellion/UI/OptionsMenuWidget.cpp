// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Tool/UtilitaryMacros.h"
//#include "Engine/HairWorksAsset.h"
#include "OptionsMenuWidget.h"
#include "Global/EntityDataSingleton.h"
#include "Global/WorldInstanceEntity.h"




UOptionsMenuWidget::UOptionsMenuWidget()
{
    //ConstructorHelpers::FObjectFinder<UObject> hairworkMaterialObject(TEXT("/Game/MixamoAnimPack/Mixamo_Maw/Materials/Maw_Hair"));
    //UHairWorksAsset* hairworksAsset = Cast<UHairWorksAsset>(hairworkMaterialObject.Object);
    //if(hairworksAsset)
    //{
    //    m_hairworksMaterial = hairworksAsset->HairMaterial;
    //}
}

void UOptionsMenuWidget::OptionsMenuCheckBox1(bool checkBoxStatus)
{
    //if(m_hairworksMaterial)
    //{
    //    m_hairworksMaterial->bEnable = checkBoxStatus;
    //}
}

void UOptionsMenuWidget::OptionsMenuCheckBox2(bool checkBoxStatus)
{
    TArray<AActor*> entity;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldInstanceEntity::StaticClass(), entity);
    if(entity.Num() > 0)
    {
        AWorldInstanceEntity* ent = Cast<AWorldInstanceEntity>(entity[0]);
        ent->setShieldAnimation(checkBoxStatus);
    }
}

void UOptionsMenuWidget::OptionsMenuCheckBox3(bool checkBoxStatus)
{
    TArray<AActor*> outResult;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), outResult);

    for(AActor* postProcessActorVolume : outResult)
    {
        APostProcessVolume* postProcessVolume = Cast<APostProcessVolume>(postProcessActorVolume);
        if(postProcessVolume && postProcessVolume->GetName() == "PostProcessVolume")
        {
            postProcessVolume->BlendWeight = checkBoxStatus ? 1.f : 0.f;
        }
    }
}

// Disable Burn Effect
void UOptionsMenuWidget::OptionsMenuCheckBox4(bool checkBoxStatus)
{
    TArray<AActor*> entity;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldInstanceEntity::StaticClass(), entity);
    if(entity.Num() > 0)
    {
        AWorldInstanceEntity* ent = Cast<AWorldInstanceEntity>(entity[0]);
        ent->setIsBurnEffectEnabled(checkBoxStatus);
    }
}

void UOptionsMenuWidget::OptionsMenuCheckBox5(bool checkBoxStatus)
{
    TArray<AActor*> outResult;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), outResult);

    for (AActor* postProcessActorVolume : outResult)
    {
        APostProcessVolume* postProcessVolume = Cast<APostProcessVolume>(postProcessActorVolume);
        if(postProcessVolume && postProcessVolume->GetName().Contains("Fog"))
        {
            postProcessVolume->BlendWeight = checkBoxStatus ? 1.f : 0.f;
        }
    }
}