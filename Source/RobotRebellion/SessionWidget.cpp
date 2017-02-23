// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LobbyUIWidget.h"
#include "SessionWidget.h"

void USessionWidget::initialiseWidget(int index, ULobbyUIWidget* parent)
{
    m_index = index;
    m_parentWidget = parent;

    auto text = GetWidgetFromName("TextButtonSession");
    if(text)
    {
        UTextBlock *tempText = Cast<UTextBlock>(text);
        tempText->SetText(FText::FromString("Session " + FString::FromInt(m_index)));
    }
}

void USessionWidget::setSelected(bool selected)
{
    m_isSelected = selected;

    UWidget *widgetButton = GetWidgetFromName("SessionButtonBackground");
    widgetButton->SetIsEnabled(selected);
}

void USessionWidget::OnClicked()
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, TEXT("Try selected session : " + FString::FromInt(m_index)));
    m_parentWidget->setSelectedSession(m_index);
    this->SetIsEnabled(false);
}
