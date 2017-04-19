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
	ARobotRebellionGameMode();

    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;
};



