// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IA/Controller/CustomAIControllerBase.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API ABossAIController : public ACustomAIControllerBase
{
	GENERATED_BODY()
	
	
public:
	/************************************************************************/
	/* UPROPERTY                                                            */
	/************************************************************************/
    /*The global ponderation coefficient for king*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA", meta = (ClampMin = 0.f, ClampMax = 1.f))
        float m_baseKingCoefficient;

    /*The global ponderation coefficient for players*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA", meta = (ClampMin = 0.f, ClampMax = 1.f))
        float m_basePlayersCoefficient;

    /*The malus applicated if the target is not in range*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA", meta = (ClampMin = 0.f, ClampMax = 1.f))
        float m_overRangeMalusCoefficient;

    /*The fall Off range when over range malus is applicated*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA", meta = (ClampMin = 1.f))
        float m_fallOffRangeCoefficient;

    /*Difficulty between 0 and 1. Higher value means higher difficulty*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA", meta = (ClampMin = 0.f, ClampMax = 1.f))
        float m_difficulty;

    /*Difficulty between 0 and 1. Higher value means higher difficulty*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA", meta = (ClampMin = 0.f))
        float m_updateTargetCooldownTime;


    /************************************************************************/
    /* PROPERTY                                                             */
    /************************************************************************/
    float m_currentKingCoeff;
    float m_currentPlayersCoeff;

    float m_lifeThreshold;

    float m_updateTargetTime;


    /************************************************************************/
    /* CONSTRUCTOR                                                          */
    /************************************************************************/
    ABossAIController();


    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/
    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;

    virtual void CheckEnnemyNear(float range) override;

    virtual void AttackTarget() const override;


private:
    void computeTarget(float range);

    float computeIndividualDistScoring(const FVector& bossPosition, const class ARobotRebellionCharacter* individual, float rangeSquared) const;

    void initializeLifeThreshold();

    void internalCheckEnnemy();
};
