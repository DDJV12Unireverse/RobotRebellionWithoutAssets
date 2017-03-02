// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextRenderComponent.h"
#include "LivingTextRenderComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ULivingTextRenderComponent : public UTextRenderComponent
{
	GENERATED_BODY()
	
	
public:
    /************************************************************************/
    /* PROPERTY                                                             */
    /************************************************************************/
    float m_currentTime;

    //spatial translation speed along Z Axis
    float m_zTranslationSpeed;

    //to save the actor position
    FVector m_savedBeginPosition;



protected:
    //A delegate that specify the way the update method of this living text object component behaves.
    void(ULivingTextRenderComponent::* m_updateMethod)(float deltaTime);



public:
    /************************************************************************/
    /* UPROPERTY                                                            */
    /************************************************************************/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_lifeTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_heightBeginRelativeToDamagedActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_heightEndRelativeToBeginHeight;



    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/
    ULivingTextRenderComponent();


    //copy the parameter. Warning : only the properties will be copied. 
    //It doesn't initialize the object => you must call initializeWith... to use the widget 
    void copyFrom(const ULivingTextRenderComponent& objectToCopyFrom);



protected:
    void doesNothing(float deltaTime)
    {}

    void updateEverything(float deltaTime);



public:
    /************************************************************************/
    /* UFUNCTION                                                            */
    /************************************************************************/
    
    //General method. Initialize this widget with a text. Must be called once for this component to be ready.
    UFUNCTION(BlueprintCallable, Category = "General")
        void initializeWithText(const FVector& actorPosition, const FString& textToDisplay, const FColor& colorToDisplay);

    //Specific method. Initialize this widget with an integer. Must be called once for this component to be ready.
    UFUNCTION(BlueprintCallable, Category = "General")
        void initializeWithInt(const FVector& actorPosition, int32 numberToDisplay, const FColor& colorToDisplay);

    /*
    Main method. Must be called regularly to refresh the displaying animation of this object. 
    This object will be destroyed at the end of its live.
    Does nothing until isReady return true.
    */
    UFUNCTION(BlueprintCallable, Category = "General")
        void update(float deltaTime)
    {
        (this->*m_updateMethod)(deltaTime);
    }

    //say if this instance is ready to be displayed and animated
    UFUNCTION(BlueprintCallable, Category = "General")
        FORCEINLINE bool isReady() const noexcept
    {
        return this->m_updateMethod != &ULivingTextRenderComponent::doesNothing && this->IsRegistered();
    }
};
