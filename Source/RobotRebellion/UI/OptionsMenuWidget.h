// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/CustomRobotRebellionUserWidget.h"
//#include "Engine/HairWorksMaterial.h"
#include "OptionsMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UOptionsMenuWidget : public UCustomRobotRebellionUserWidget
{
    GENERATED_BODY()

private:
    //UHairWorksMaterial* m_hairworksMaterial;

public:
        UOptionsMenuWidget();

        UFUNCTION(BlueprintCallable, Category = "Menu")
            void OptionsMenuCheckBox1(bool checkBoxStatus);

        UFUNCTION(BlueprintCallable, Category = "Menu")
            void OptionsMenuCheckBox2(bool checkBoxStatus);

        UFUNCTION(BlueprintCallable, Category = "Menu")
            void OptionsMenuCheckBox3(bool checkBoxStatus);

        UFUNCTION(BlueprintCallable, Category = "Menu")
            void OptionsMenuCheckBox4(bool checkBoxStatus);

        UFUNCTION(BlueprintCallable, Category = "Menu")
            void OptionsMenuCheckBox5(bool checkBoxStatus);


};
