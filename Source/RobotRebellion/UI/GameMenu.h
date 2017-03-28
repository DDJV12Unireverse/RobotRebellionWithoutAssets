// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "LobbyUIWidget.h"
#include "ReviveTimerWidget.h"
#include "CustomRobotRebellionUserWidget.h"
#include "RobotRebellionWidget.h"
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Game Menu Lobby Widget")
        TSubclassOf<ULobbyUIWidget> LobbyWidget;
    ULobbyUIWidget* LobbyImpl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD Character Widget")
        TSubclassOf<UCustomRobotRebellionUserWidget> HUDCharacterWidget;
    UCustomRobotRebellionUserWidget* HUDCharacterImpl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Revive Timer Widget")
        TSubclassOf<UReviveTimerWidget> ReviveWidget;
    UReviveTimerWidget* ReviveTimerWidgetImpl;

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
     
};
