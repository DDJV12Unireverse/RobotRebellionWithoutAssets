// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "TopWidget.h"
#include "GameMenu.h"




void UTopWidget::SinglePlayerGame()
{
    APlayerController *  MyPC = GetOwningPlayer();

    if(MyPC)
    {
        auto myHud = Cast<AGameMenu>(MyPC->GetHUD());
        if(myHud->ClassSelectionWidgetImpl->IsVisible())
        {
            CloseSinglePlayerGameWidget();
            //return;
        }
        myHud->HideWidget(myHud->TopWidgetImpl);
        myHud->DisplayWidget(myHud->ClassSelectionWidgetImpl);
        //giveInputGameMode(false);
    }
}

void UTopWidget::CloseSinglePlayerGameWidget()
{
    APlayerController *  MyPC = GetOwningPlayer();
    if(MyPC)
    {
        auto myHud = Cast<AGameMenu>(MyPC->GetHUD());
        myHud->HideWidget(myHud->TopWidgetImpl);
        //giveInputGameMode(true);
    }
}

void UTopWidget::NetworkPlayerGame()
{
    APlayerController *  MyPC = GetOwningPlayer();

    if(MyPC)
    {
        auto myHud = Cast<AGameMenu>(MyPC->GetHUD());
        if(myHud->LobbyImpl->IsVisible())
        {
            //CloseSinglePlayerGameWidget();
            //return;
        }
        myHud->HideWidget(myHud->TopWidgetImpl);
        myHud->HideWidget(myHud->ClassSelectionWidgetImpl);
        myHud->DisplayWidget(myHud->LobbyImpl);
        //giveInputGameMode(false);
    }
}

void UTopWidget::GameOptionsMenu()
{
    APlayerController *  MyPC = GetOwningPlayer();

    if(MyPC)
    {
        auto myHud = Cast<AGameMenu>(MyPC->GetHUD());
        if(myHud->OptionsWidgetImpl->IsVisible())
        {
            CloseGameOptionsMenu();
        }
        myHud->HideWidget(myHud->TopWidgetImpl);
        myHud->DisplayWidget(myHud->OptionsWidgetImpl);
    }
}

void UTopWidget::CloseGameOptionsMenu()
{
    APlayerController *  MyPC = GetOwningPlayer();
    if(MyPC)
    {
        auto myHud = Cast<AGameMenu>(MyPC->GetHUD());
        myHud->HideWidget(myHud->OptionsWidgetImpl);
    }
}

void UTopWidget::setReturnInGameVisible(bool enable)
{
    auto widget = this->GetWidgetFromName(TEXT("ReturnToGameButton"));
    if(enable)
    {
        widget->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        widget->SetVisibility(ESlateVisibility::Hidden);
    }
};