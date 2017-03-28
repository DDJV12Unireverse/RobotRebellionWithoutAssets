// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "GameInstaller.h"
#include "RobotRebellionGameMode.generated.h"

UCLASS(minimalapi)
class ARobotRebellionGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alteration Default")
        TSubclassOf<class UAlterationBase> m_invisibleDefault;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alteration Default")
        TSubclassOf<class UAlterationBase> m_stunDefault;

public:
	ARobotRebellionGameMode();

    virtual void BeginPlay() override;
};



