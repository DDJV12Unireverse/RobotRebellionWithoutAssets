// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "TextBillboardComponent.h"

#include "LivingTextRenderComponent.h"
#include "UtilitaryFunctionLibrary.h"


UTextBillboardComponent::UTextBillboardComponent() : UBillboardComponent()
{
    bAutoActivate = true;
}

void UTextBillboardComponent::beginDisplayingText(const FVector& actorPositionInWorld, const FString& text, const FColor& colorToDisplay)
{
    ULivingTextRenderComponent* intermediary;

    if (UUtilitaryFunctionLibrary::createObjectFromDefault<ULivingTextRenderComponent>(&intermediary, m_defaultRenderText, this))
    {
        intermediary->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

        intermediary->Activate();

        m_damageRenderedTextArray.Add(intermediary);
        m_damageRenderedTextArray.Top()->initializeWithText(actorPositionInWorld, text, colorToDisplay);
    }
}

void UTextBillboardComponent::beginDisplayingInteger(const FVector& actorPositionInWorld, int32 integerValue, const FColor& colorToDisplay)
{
    this->beginDisplayingText(actorPositionInWorld, FString::FromInt(integerValue), colorToDisplay);
}

void UTextBillboardComponent::update(float deltaTime)
{
    if (this->nothingToRender())
    {
        return;
    }

    for (auto iter = 0; iter < m_damageRenderedTextArray.Num(); ++iter)
    {
        m_damageRenderedTextArray[iter]->update(deltaTime);
    }

    m_damageRenderedTextArray.RemoveAll(
        [](ULivingTextRenderComponent* current) { 
            return current == NULL || current->IsPendingKillOrUnreachable(); 
    });

    this->UpdateChildTransforms();
}
