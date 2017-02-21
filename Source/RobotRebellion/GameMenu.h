// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "LobbyUIWidget.h"
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD Game Menu Widget")
        TSubclassOf<ULobbyUIWidget> LobbyWidget;

    template <class T>
    void DisplayWidget(T *Widget)
    {
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Creation widget | BEGIN"));
        }
        if(Widget)
        {
            T* WidgetToImp = CreateWidget<T>(GetWorld(), Widget->GetClass());

            if(GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Creation widget | TEST"));
            }
            /** Make sure widget was created */
            if(WidgetToImp)
            {
                /** Add it to the viewport */
                WidgetToImp->AddToViewport();

                if(GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Creation widget | DONE"));
                }
            }
        }
    }

    //template <class T>
    //void HideWidget(T *Widget)
    //{
    //    if(GEngine)
    //    {
    //        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hide widget | BEGIN"));
    //    }
    //    if(Widget->IsInViewport())
    //    {
    //        Widget->RemoveFromParent();
    //        if(GEngine)
    //        {
    //            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hide widget | DONE"));
    //        }
    //    }
    //}
};
