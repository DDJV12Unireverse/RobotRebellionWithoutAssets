// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LivingTextRenderComponent.h"


ULivingTextRenderComponent::ULivingTextRenderComponent() : UTextRenderComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    bAutoActivate = true;

    m_updateMethod = &ULivingTextRenderComponent::doesNothing;
}

void ULivingTextRenderComponent::initializeWithText(const FVector& actorPosition, const FString& textToDisplay, const FColor& colorToDisplay)
{
    //set the current time to 0
    m_currentTime = 0.f;

    //set the damage in the text
    this->SetText(FText::FromString(textToDisplay));

    this->SetTextRenderColor(colorToDisplay);

    //put the component at the position to display the text
    m_savedBeginPosition = actorPosition;
    m_savedBeginPosition.Z += m_heightBeginRelativeToDamagedActor;
    this->SetWorldLocation(actorPosition);
    this->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

    //Now speeds. For those in Unireverse that are not familiar with speed computation
    // v = d / t
    m_zTranslationSpeed = m_heightEndRelativeToBeginHeight / m_lifeTime;

    //Now that everything is ready, we can begin to really update everything until this component dies...
    this->m_updateMethod = &ULivingTextRenderComponent::updateEverything;

    //Registration. Important !!
    this->RegisterComponent();
}

void ULivingTextRenderComponent::initializeWithInt(const FVector& actorPosition, int32 numberToDisplay, const FColor& colorToDisplay)
{
    this->initializeWithText(actorPosition, FString::FromInt(numberToDisplay), colorToDisplay);
}

void ULivingTextRenderComponent::updateEverything(float deltaTime)
{
    if (IsPendingKillOrUnreachable())
    {
        return;
    }

    m_currentTime += deltaTime;

    if (m_currentTime < m_lifeTime)
    {
        m_savedBeginPosition.Z += m_zTranslationSpeed * deltaTime;

        this->SetWorldLocation(m_savedBeginPosition);
    }
    else
    {
        this->UnregisterComponent();
        this->DestroyComponent();
    }
}

void ULivingTextRenderComponent::copyFrom(const ULivingTextRenderComponent& objectToCopyFrom)
{
    m_lifeTime = objectToCopyFrom.m_lifeTime;
    m_heightBeginRelativeToDamagedActor = objectToCopyFrom.m_heightBeginRelativeToDamagedActor;
    m_heightEndRelativeToBeginHeight = objectToCopyFrom.m_heightEndRelativeToBeginHeight;

    this->m_updateMethod = &ULivingTextRenderComponent::doesNothing;
}
