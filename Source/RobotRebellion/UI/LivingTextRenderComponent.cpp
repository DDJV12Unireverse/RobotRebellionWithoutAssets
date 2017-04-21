// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LivingTextRenderComponent.h"

#define PI_MUL_3 9.42477796077f


ULivingTextRenderComponent::ULivingTextRenderComponent() : UTextRenderComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    bReplicates = true;

    bAutoActivate = true;

    this->m_updateMethod = &ULivingTextRenderComponent::doesNothing;
}

void ULivingTextRenderComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ULivingTextRenderComponent::initializeWithText(const FVector& actorPosition, const FString& textToDisplay, const FColor& colorToDisplay, ELivingTextAnimMode mode)
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
    this->setDelegateAccordingToAnimMode(mode);

    //Registration. Important !!
    this->RegisterComponent();
}

void ULivingTextRenderComponent::initializeWithInt(const FVector& actorPosition, int32 numberToDisplay, const FColor& colorToDisplay, ELivingTextAnimMode mode)
{
    this->initializeWithText(actorPosition, FString::FromInt(numberToDisplay), colorToDisplay, mode);
}

void ULivingTextRenderComponent::updateEverything(float deltaTime)
{
    if (IsPendingKillOrUnreachable())
    {
        return;
    }

    m_currentTime += deltaTime;

    if (!this->isAtEndOfLife())
    {
        m_savedBeginPosition.Z += m_zTranslationSpeed * deltaTime;

        this->SetWorldLocation(m_savedBeginPosition);
    }
    else
    {
        destroyLivingText();
    }
}

void ULivingTextRenderComponent::updateWithoutMoving(float deltaTime)
{
    if (IsPendingKillOrUnreachable())
    {
        return;
    }

    m_currentTime += deltaTime;

    if (!this->isAtEndOfLife())
    {
        this->SetWorldLocation(m_savedBeginPosition);
    }
    else
    {
        this->destroyLivingText();
    }
}

void ULivingTextRenderComponent::updateBoingBoing(float deltaTime)
{
    if (IsPendingKillOrUnreachable())
    {
        return;
    }

    m_currentTime += deltaTime;

    if (!this->isAtEndOfLife())
    {
        float animationTransfertMethod = m_currentTime * PI_MUL_3 / m_lifeTime; //normalized time over 3pi

        //sin(u)/u make it boing boing ^^
        animationTransfertMethod = FMath::Sin(animationTransfertMethod) / animationTransfertMethod;
        animationTransfertMethod /= 64.f; //vertical scaling

        m_savedBeginPosition.Z += m_zTranslationSpeed * animationTransfertMethod;

        this->SetWorldLocation(m_savedBeginPosition);
    }
    else
    {
        this->destroyLivingText();
    }
}

void ULivingTextRenderComponent::updateBoingBiggerText(float deltaTime)
{
    if(IsPendingKillOrUnreachable())
    {
        return;
    }

    m_currentTime += deltaTime;

    if(!this->isAtEndOfLife())
    {
        m_savedBeginPosition.Z += m_zTranslationSpeed * deltaTime;
        this->SetWorldLocation(m_savedBeginPosition);

        float animationTransfertMethod = m_currentTime * PI_MUL_3 / m_lifeTime; //normalized time over 6 PI
        animationTransfertMethod = FMath::Abs(FMath::Sin(animationTransfertMethod)) + 0.5f; //result will be between 0.3f and 1.3f
        //this->SetRelativeScale3D({ this->RelativeScale3D.X, animationTransfertMethod, animationTransfertMethod });

        this->SetXScale(animationTransfertMethod);
        this->SetYScale(animationTransfertMethod);
    }
    else
    {
        this->destroyLivingText();
    }
}

void ULivingTextRenderComponent::copyFrom(const ULivingTextRenderComponent& objectToCopyFrom)
{
    m_lifeTime = objectToCopyFrom.m_lifeTime;
    m_heightBeginRelativeToDamagedActor = objectToCopyFrom.m_heightBeginRelativeToDamagedActor;
    m_heightEndRelativeToBeginHeight = objectToCopyFrom.m_heightEndRelativeToBeginHeight;

    setDelegateAccordingToAnimMode(ELivingTextAnimMode::TEXT_ANIM_NOT_READY);
}

void ULivingTextRenderComponent::setDelegateAccordingToAnimMode(ELivingTextAnimMode mode)
{
    static void(ULivingTextRenderComponent::*const delegateArrayMapperLookUpTable[])(float) = 
    {
        &ULivingTextRenderComponent::doesNothing,
        &ULivingTextRenderComponent::updateEverything,
        &ULivingTextRenderComponent::updateWithoutMoving,
        &ULivingTextRenderComponent::updateBoingBoing,
        &ULivingTextRenderComponent::updateBoingBiggerText
    };

    this->m_updateMethod = delegateArrayMapperLookUpTable[static_cast<uint8>(mode)];
}

void ULivingTextRenderComponent::destroyLivingText()
{
    this->UnregisterComponent();
    this->DestroyComponent();
}