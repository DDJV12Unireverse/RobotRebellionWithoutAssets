// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "CustomRobotRebellionUserWidget.h"



void UCustomRobotRebellionUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    this->updateEveryFrame();
}

void UCustomRobotRebellionUserWidget::updateEveryFrame()
{

}