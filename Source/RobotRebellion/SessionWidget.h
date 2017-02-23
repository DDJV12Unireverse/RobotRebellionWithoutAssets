// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "SessionWidget.generated.h"

class ULobbyUIWidget;
/**
 *
 */
UCLASS()
class ROBOTREBELLION_API USessionWidget : public UUserWidget
{
    GENERATED_BODY()

private:
    bool m_isSelected;
    int m_index;
    ULobbyUIWidget* m_parentWidget;

public:
    void initialiseWidget(int index, ULobbyUIWidget* parent);
    void setSelected(bool selected = true);
    bool isSelected() const USE_NOEXCEPT
    {
        return m_isSelected;
    }
    UFUNCTION(BlueprintCallable, Category = SessionWidget)
        void OnClicked();
    UFUNCTION(BlueprintCallable, Category = SessionWidget)
        int getIndex() const
    {
        return m_index;
    }
};