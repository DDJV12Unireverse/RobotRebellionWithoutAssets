// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BillboardComponent.h"
#include "ELivingTextAnimMode.h"
#include "TextBillboardComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API UTextBillboardComponent : public UBillboardComponent
{
	GENERATED_BODY()
	
	
private:
    /************************************************************************/
    /* UPROPERTY                                                            */
    /************************************************************************/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display", meta = (AllowPrivateAccess = "true"))
        TSubclassOf<class ULivingTextRenderComponent> m_defaultRenderText;



    /************************************************************************/
    /*PROPERTY                                                              */
    /************************************************************************/
    TArray<class ULivingTextRenderComponent*> m_damageRenderedTextArray;



public:
    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/
    UTextBillboardComponent();


    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


    /************************************************************************/
    /* UFUNCTION                                                            */
    /************************************************************************/

    /*
    General method. create and begin displaying a living (animated) text (given by text) at the specified location.
    */
    UFUNCTION(BlueprintCallable, Category = "CUSTOM BillBoard Component")
        void beginDisplayingText(const FVector& actorPositionInWorld, const FString& text, const FColor& colorToDisplay, ELivingTextAnimMode mode = ELivingTextAnimMode::TEXT_ANIM_MOVING);

    /*
    Specific method. create and begin displaying a living (animated) text (given by the integerValue) at the specified location.
    */
    UFUNCTION(BlueprintCallable, Category = "CUSTOM BillBoard Component")
        void beginDisplayingInteger(const FVector& actorPositionInWorld, int32 integerValue, const FColor& colorToDisplay, ELivingTextAnimMode mode = ELivingTextAnimMode::TEXT_ANIM_MOVING);

    /*
    Main method. Called to refresh all animation and living text. Must be called regularly
    */
    UFUNCTION(BlueprintCallable, Category = "CUSTOM BillBoard Component")
        void update(float deltaTime);

    /*
    Clear all texts and remove them
    */
    UFUNCTION(BlueprintCallable, Category = "CUSTOM BillBoard Component")
        void clearAllLivingTexts();

    //Gives the number of living text this billboard currently has to render.
    UFUNCTION(BlueprintCallable, Category = "CUSTOM BillBoard Component")
        FORCEINLINE int32 livingTextCountToRender() const USE_NOEXCEPT
    {
        return m_damageRenderedTextArray.Num();
    }

    //return true if this blackboard is empty (has no living text to display)
    UFUNCTION(BlueprintCallable, Category = "CUSTOM BillBoard Component")
        FORCEINLINE bool nothingToRender() const USE_NOEXCEPT
    {
        return livingTextCountToRender() == 0;
    }
};
