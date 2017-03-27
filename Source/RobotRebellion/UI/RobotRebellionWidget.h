// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "RobotRebellionWidget.generated.h"

/**
 *
 */
UCLASS()
class ROBOTREBELLION_API URobotRebellionWidget : public UUserWidget
{
    GENERATED_BODY()

private:
    UPROPERTY(VisibleDefaultsOnly)
        class APlayableCharacter* m_owner;
    UPROPERTY(VisibleDefaultsOnly)
        UAudioComponent* m_notifyAudioComp;
    UPROPERTY(VisibleDefaultsOnly)
        UAudioComponent* m_loopAudioComp;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetSound")
        bool m_stopAmbiantSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetSound")
        USoundCue* m_widgetBeginSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetSound")
        USoundCue* m_widgetLoopSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetSound")
        USoundCue* m_widgetCloseSound;


    UFUNCTION()
        void initializeOwner();
    UFUNCTION(BlueprintCallable, Category = "WidgetSound")
        virtual void startSound();
    UFUNCTION(BlueprintCallable, Category = "WidgetSound")
        virtual void endSound();

};
