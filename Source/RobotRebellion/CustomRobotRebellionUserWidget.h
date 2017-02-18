// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CustomRobotRebellionUserWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROBOTREBELLION_API UCustomRobotRebellionUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    //I override the NativeTick method
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


    //Implement in the blueprint
    UFUNCTION(BlueprintCallable, Category = "UpdateMethod")
        virtual void updateEveryFrame(); 
};
