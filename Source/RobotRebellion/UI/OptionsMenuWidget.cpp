// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Tool/UtilitaryMacros.h"
#include "OptionsMenuWidget.h"
#include "Global/EntityDataSingleton.h"
#include "Global/WorldInstanceEntity.h"




void UOptionsMenuWidget::OptionsMenuCheckBox1(bool checkBoxStatus)
{
    // TODO: Set a variable used by your animtion/rendering fx or call a method.
}

void UOptionsMenuWidget::OptionsMenuCheckBox2(bool checkBoxStatus)
{}

void UOptionsMenuWidget::OptionsMenuCheckBox3(bool checkBoxStatus)
{}

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