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
    ULobbyUIWidget* m_parentWidget;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SessionWidget, meta = (AllowPrivateAccess = "true"))
    int m_index;

public:
    void initialiseWidget(int index, ULobbyUIWidget* parent);
    void setSelected(bool selected = true);

    UFUNCTION(BlueprintCallable, Category = SessionWidget)
        void OnClicked();
    UFUNCTION(BlueprintCallable, Category = SessionWidget)
        int getIndex() const
    {
        return m_index;
    }
};