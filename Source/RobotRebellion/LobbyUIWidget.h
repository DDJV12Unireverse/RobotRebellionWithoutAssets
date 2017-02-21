// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "LobbyUIWidget.generated.h"

class ARobotRebellionCharacter;
/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ULobbyUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyServer | Settings")
        FString m_widgetName;

    UFUNCTION(BlueprintCallable, Category = "LobbyServer | Function")
        void CreateServer(FString mapName);

    UFUNCTION(BlueprintCallable, Category = "LobbyServer | Function")
        void JoinServer(FString IPAdress);
	
	
};
