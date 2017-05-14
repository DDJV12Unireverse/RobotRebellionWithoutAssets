// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Tool/UtilitaryMacros.h"
#include "Engine/HairWorksAsset.h"
#include "OptionsMenuWidget.h"




UOptionsMenuWidget::UOptionsMenuWidget()
{
    ConstructorHelpers::FObjectFinder<UObject> hairworkMaterialObject(TEXT("/Game/MixamoAnimPack/Mixamo_Maw/Materials/Maw_Hair"));
    UHairWorksAsset* hairworksAsset = Cast<UHairWorksAsset>(hairworkMaterialObject.Object);
    if(hairworksAsset)
    {
        m_hairworksMaterial = hairworksAsset->HairMaterial;
    }
}

void UOptionsMenuWidget::OptionsMenuCheckBox1(bool checkBoxStatus)
{
    if(m_hairworksMaterial)
    {
        m_hairworksMaterial->bEnable = checkBoxStatus;
    }
}

void UOptionsMenuWidget::OptionsMenuCheckBox2(bool checkBoxStatus)
{}

void UOptionsMenuWidget::OptionsMenuCheckBox3(bool checkBoxStatus)
{}

void UOptionsMenuWidget::OptionsMenuCheckBox4(bool checkBoxStatus)
{}

void UOptionsMenuWidget::OptionsMenuCheckBox5(bool checkBoxStatus)
{
    TArray<AActor*> outResult;
    TSubclassOf<APostProcessVolume> classToSearchFor{ APostProcessVolume::StaticClass() };

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToSearchFor, outResult);

    for (AActor* postProcessActorVolume : outResult)
    {
        APostProcessVolume* postProcessVolume = Cast<APostProcessVolume>(postProcessActorVolume);
        if(postProcessVolume && postProcessVolume->GetName().Contains("Fog"))
        {
            postProcessVolume->BlendWeight = checkBoxStatus ? 1.f : 0.f;
        }
    }
}