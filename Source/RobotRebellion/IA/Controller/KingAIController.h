// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IA/Controller/CustomAIControllerBase.h"
#include "KingAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API AKingAIController : public ACustomAIControllerBase
{
	GENERATED_BODY()
	
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_ennemyCoefficient;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_moveThreshold;


    FVector m_destination;

    void (AKingAIController::* m_updateMethodPtr)();


    virtual void BeginPlay() override;
    virtual void Tick(float deltaTime) override;

    virtual EPathFollowingRequestResult::Type MoveToTarget() override;


    FORCEINLINE UFUNCTION(BlueprintCallable, Category = "King")
        void activate(bool isKingFree) USE_NOEXCEPT
    {
        this->m_updateMethodPtr = isKingFree ? &AKingAIController::updateKing : &AKingAIController::doesNothing;
    }

    FORCEINLINE UFUNCTION(BlueprintCallable, Category = "King")
        bool isActivated() const USE_NOEXCEPT
    {
        return this->m_updateMethodPtr == &AKingAIController::updateKing;
    }


private:
    void doesNothing()
    {}

    void updateKing();

    void computeTarget();
};
