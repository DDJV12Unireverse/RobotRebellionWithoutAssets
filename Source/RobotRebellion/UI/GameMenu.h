// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "LobbyUIWidget.h"
#include "CustomRobotRebellionUserWidget.h"
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Game Menu Lobby Widget")
        TSubclassOf<ULobbyUIWidget> LobbyWidget;
    ULobbyUIWidget* LobbyImpl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD Character Widget")
        TSubclassOf<UCustomRobotRebellionUserWidget> HUDCharacterWidget;
    UCustomRobotRebellionUserWidget* HUDCharacterImpl;

    template <class T>
    T* CreateCustomWidget(T *Widget)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, TEXT("Creation widget | BEGIN"));
        if(Widget)
        {
            T* WidgetToImp = CreateWidget<T>(GetWorld(), Widget->GetClass());

            PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, TEXT("Creation widget | TEST"));
            /** Make sure widget was created */
            if(WidgetToImp)
            {
                /** Add it to the viewport */
                WidgetToImp->AddToViewport();

                PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, TEXT("Creation widget | DONE"));
                return WidgetToImp;
            }
        }
        return nullptr;
    }

    template <class T>
    void RemoveWidget(T *WidgetRef)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, TEXT("Hide widget | Begin"));

        if(WidgetRef->IsInViewport())
        {
            WidgetRef->RemoveFromParent();

            PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, TEXT("Hide widget | DONE"));
        }
    }

    void DisplayWidget(UUserWidget* WidgetRef)
    {
        WidgetRef->SetVisibility(ESlateVisibility::Visible);
    }

    void HideWidget(UUserWidget* WidgetRef)
    {
        WidgetRef->SetVisibility(ESlateVisibility::Hidden);
    }
};
