// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CustomAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ACustomAIControllerBase : public AAIController
{
	GENERATED_BODY()
        
protected:
    class ARobotRebellionCharacter *m_targetToFollow;

public :
    virtual EPathFollowingRequestResult::Type MoveToTarget();

    bool hasTarget() const USE_NOEXCEPT
    {
        return m_targetToFollow != NULL;
    }
	
};
