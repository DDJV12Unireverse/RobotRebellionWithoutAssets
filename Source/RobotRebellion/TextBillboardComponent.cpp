// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "TextBillboardComponent.h"

#include "LivingTextRenderComponent.h"
#include "UtilitaryFunctionLibrary.h"


UTextBillboardComponent::UTextBillboardComponent() : UBillboardComponent()
{
    bAutoActivate = true;

    bReplicates = true;
}

void UTextBillboardComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UTextBillboardComponent::beginDisplayingText(const FVector& actorPositionInWorld, const FString& text, const FColor& colorToDisplay, ELivingTextAnimMode mode)
{
    ULivingTextRenderComponent* intermediary;

    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Cyan, "BEGIN DISPLAY UTextBillboardComponent");

    if (UUtilitaryFunctionLibrary::createObjectFromDefault<ULivingTextRenderComponent>(&intermediary, m_defaultRenderText, this, RF_Dynamic))
    {
        intermediary->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

        intermediary->Activate();

        m_damageRenderedTextArray.Add(intermediary);
        m_damageRenderedTextArray.Top()->initializeWithText(actorPositionInWorld, text, colorToDisplay, mode);
    }
}

void UTextBillboardComponent::beginDisplayingInteger(const FVector& actorPositionInWorld, int32 integerValue, const FColor& colorToDisplay, ELivingTextAnimMode mode)
{
    this->beginDisplayingText(actorPositionInWorld, FString::FromInt(integerValue), colorToDisplay, mode);
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
            return 
                current == NULL || 
                current->IsBeingDestroyed() || 
                current->IsPendingKillOrUnreachable() || 
                !current->isReady();
    });

    this->UpdateChildTransforms();
}

void UTextBillboardComponent::clearAllLivingTexts()
{
    for (auto iter = 0; iter < m_damageRenderedTextArray.Num(); ++iter)
    {
        m_damageRenderedTextArray[iter]->destroyLivingText();
    }

    m_damageRenderedTextArray.Empty();
}