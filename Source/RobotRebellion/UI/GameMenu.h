// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "LobbyUIWidget.h"
#include "ReviveTimerWidget.h"
#include "CustomRobotRebellionUserWidget.h"
#include "RobotRebellionWidget.h"
#include "TopWidget.h"
#include "OptionsMenuWidget.h"
#include "GameMenu.generated.h"

/**
 *
 */
UCLASS()
class ROBOTREBELLION_API AGameMenu : public AHUD
{
    GENERATED_BODY()

public:
    AGameMenu();
    virtual void BeginPlay() override;
    virtual void Tick(float deltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Top Game Menu")
        TSubclassOf<UTopWidget> TopWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget | Top Game Menu")
        UTopWidget* TopWidgetImpl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | UI Game Menu Lobby")
        TSubclassOf<ULobbyUIWidget> LobbyWidget;
    ULobbyUIWidget* LobbyImpl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | HUD Character")
        TSubclassOf<UCustomRobotRebellionUserWidget> HUDCharacterWidget;
    UCustomRobotRebellionUserWidget* HUDCharacterImpl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Revive Timer")
        TSubclassOf<UReviveTimerWidget> ReviveWidget;
    UReviveTimerWidget* ReviveTimerWidgetImpl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Classes Selection")
        TSubclassOf<URobotRebellionWidget> ClassSelectionWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget | Classes Selection")
        URobotRebellionWidget* ClassSelectionWidgetImpl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Options Menu")
        TSubclassOf<UOptionsMenuWidget> OptionsWidget;
    UOptionsMenuWidget* OptionsWidgetImpl;

    //TODO OPTIONS MENU

    template <class T>
    T* CreateCustomWidget(T *Widget)
    {
        if(Widget)
        {
            T* WidgetToImp = CreateWidget<T>(GetWorld(), Widget->GetClass());

            /** Make sure widget was created */
            if(WidgetToImp)
            {
                /** Add it to the viewport */
                WidgetToImp->AddToViewport();

                return WidgetToImp;
            }
        }
        return nullptr;
    }

    template <class T>
    void RemoveWidget(T *WidgetRef)
    {

        if(WidgetRef->IsInViewport())
        {
            WidgetRef->RemoveFromParent();
        }
    }

    UFUNCTION(BlueprintCallable, Category = HUD)
        void DisplayWidget(URobotRebellionWidget* WidgetRef);

    UFUNCTION(BlueprintCallable, Category = HUD)
        void HideWidget(URobotRebellionWidget* WidgetRef);

    UFUNCTION(BlueprintNativeEvent, Category = "Methods")
        void firstCallHUD();

};
