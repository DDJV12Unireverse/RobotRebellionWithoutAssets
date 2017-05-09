// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "BossAIController.h"

#include "Global/EntityDataSingleton.h"
#include "Character/PlayableCharacter.h"
#include "Character/RobotRebellionCharacter.h"
#include "Character/Drone.h"
#include "Character/King.h"
#include "Gameplay/Weapon/WeaponBase.h"
#include "Kismet/KismetMathLibrary.h"



ABossAIController::ABossAIController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABossAIController::BeginPlay()
{
    Super::BeginPlay();

    this->initializeLifeThreshold();
}

void ABossAIController::Tick(float deltaTime)
{
    if (this->IsPendingKillOrUnreachable())
    {
        return;
    }

    Super::Tick(deltaTime);

    m_updateTargetTime += deltaTime;

    if(m_updateTargetTime < m_updateTargetCooldownTime)
    {
        return;
    }

    m_updateTargetTime = 0.f;
    this->internalCheckEnnemy();
}

void ABossAIController::internalCheckEnnemy()
{
    ARobotRebellionCharacter* boss = Cast<ARobotRebellionCharacter>(GetPawn());

    if (boss)
    {
        this->computeTarget(boss->getCurrentEquippedWeapon()->m_WeaponRadiusRange);
    }
}

void ABossAIController::CheckEnnemyNear(float range)
{
    //...
}

void ABossAIController::AttackTarget() const
{
    ANonPlayableCharacter* boss = Cast<ANonPlayableCharacter>(GetPawn());

    if(this->getTarget())
    {
        FVector hitDirection = UKismetMathLibrary::GetForwardVector(
            UKismetMathLibrary::FindLookAtRotation(boss->GetActorLocation(), this->getTarget()->GetActorLocation())
        );

        hitDirection.Z = 0.f;
        hitDirection.Normalize();

        FVector front = boss->GetActorForwardVector();
        front.Z = 0.f;
        front.Normalize();

        FVector vert = FVector::CrossProduct(front, hitDirection);
        float sinAngle = vert.Size();

        if(vert.Z < 0.f)
        {
            sinAngle = -sinAngle;
        }

        boss->AddActorLocalRotation(FQuat({ 0.f, 0.f, 1.f }, asinf(sinAngle)), true);

        boss->getCurrentEquippedWeapon()->cppAttack(boss, this->getTarget());
    }
    else
    {
        boss->getCurrentEquippedWeapon()->cppAttack(boss);
    }
}

void ABossAIController::initializeLifeThreshold()
{
    constexpr const float MIN_THRESHOLD = 0.15f;
    constexpr const float MAX_THRESHOLD = 0.55f;

    constexpr const float DELTA_THRESHOLD = MAX_THRESHOLD - MIN_THRESHOLD;

    m_lifeThreshold = m_difficulty * DELTA_THRESHOLD + MIN_THRESHOLD;
}

float ABossAIController::computeIndividualDistScoring(const FVector& bossPosition, const ARobotRebellionCharacter* individual, float rangeSquared) const
{
    FVector potentialTargetPosition = individual->GetActorLocation();

    float deltaDist = FVector::DistSquared(bossPosition, potentialTargetPosition) - rangeSquared;

    if(deltaDist < 0.f)
    {
        return 1.f;
    }

    float score = 1.f - (m_overRangeMalusCoefficient * deltaDist / m_fallOffRangeCoefficient);

    return score < 0.f ? 0.f : score;
}

void ABossAIController::computeTarget(float range)
{
    this->setTarget(nullptr);

    FVector currentPosition = GetPawn()->GetActorLocation();

    range *= range;

    const EntityDataSingleton& datas = EntityDataSingleton::getInstance();
    const TArray<APlayableCharacter*>& players = datas.m_playableCharacterArray;

    //////////////////////////////////////////////////////////////////////////
    //Array
    static constexpr const float classificationArray[] = {
        0.8f, // Soldat
        0.5f, // Assassin
        0.4f, // Healer
        1.f, // Wizard
        0.45f // King
    };


    //////////////////////////////////////////////////////////////////////////
    //Methods

    auto getLifeRatioFunc = [](const ARobotRebellionCharacter* individual) {
        return individual->getHealth() / individual->getMaxHealth();
    };

    auto computeLifeScore = [getLifeRatioFunc](const ARobotRebellionCharacter* individual, float difficulty) {
        float lifeRatio = getLifeRatioFunc(individual);

        return
            difficulty * (1.f - lifeRatio) +
            (1.f - difficulty) * lifeRatio;
    };

    auto getDiversScore = [&datas](const ARobotRebellionCharacter* individual) {
        const APlayableCharacter* isPlayer = Cast<APlayableCharacter>(individual);

        bool droneLoaded = datas.m_drone->isLoaded();
        if (isPlayer)
        {
            return 
                ((!droneLoaded) ? 0.5f : 0.f) +
                (isPlayer->getReviveTimer() ? 0.5f : 0.f);
        }

        return droneLoaded ? 1.f : 0.f;
    };

    auto getClassifiedScore = [](const ARobotRebellionCharacter* individual, const float* classificationArray) {
        const APlayableCharacter* carac = Cast<APlayableCharacter>(individual);
        if (carac)
        {
            return classificationArray[static_cast<int32>(carac->getType()) - 1];
        }
        return classificationArray[4];
    };

    auto computeIndividualScore =
        [this, computeLifeScore, getClassifiedScore, getDiversScore, &currentPosition, range]
    (const ARobotRebellionCharacter* individual, const float* classificationArray) {
        return
            0.51f * this->computeIndividualDistScoring(currentPosition, individual, range) +
            0.26f * computeLifeScore(individual, this->m_difficulty) +
            0.19f * getClassifiedScore(individual, classificationArray) +
            0.06f * getDiversScore(individual) +
            0.01f * FMath::RandRange(0.f, 1.f); //to make a difference with someone with the exact same score
    };


    //////////////////////////////////////////////////////////////////////////
    //Computation

    int32 playerAlive = 0;
    float maxPlayerScore = 0.f;

    ARobotRebellionCharacter* chosenPlayer = nullptr;

    for(APlayableCharacter* player : players)
    {
        if(!player->isDead() && player->isVisible())
        {
            ++playerAlive;

            float playersScoreTemp = computeIndividualScore(player, classificationArray);

            if(playersScoreTemp > maxPlayerScore)
            {
                maxPlayerScore = playersScoreTemp;
                chosenPlayer = player;
            };
        }
    }

    //The more the players, the more the boss will target players. Tweeked by difficulty.
    maxPlayerScore =
        0.81f * maxPlayerScore +
        0.19f * (m_difficulty * static_cast<float>(playerAlive) / static_cast<float>(players.Num()));

    float kingScore = -1.f;

    if (datas.m_king)
    {
        kingScore = computeIndividualScore(datas.m_king, classificationArray);
    }

    ARobotRebellionCharacter* boss = Cast<ARobotRebellionCharacter>(GetPawn());

    float bossLifeRatio = getLifeRatioFunc(boss);

    //uncomment if you want to tweek difficulty at runtime
    //this->initializeLifeThreshold();

    //more hp the boss has, more it will target players. Tweeked by difficulty
    if (bossLifeRatio < m_lifeThreshold)
    {
        maxPlayerScore *= 0.70f;
    }

    maxPlayerScore *= m_basePlayersCoefficient;
    kingScore *= m_baseKingCoefficient;

    this->setTarget((maxPlayerScore > kingScore && chosenPlayer) ? chosenPlayer : datas.getServerKing(this));

    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Yellow, FString::Printf(TEXT("Player : %f, King : %f"), maxPlayerScore, kingScore));
}