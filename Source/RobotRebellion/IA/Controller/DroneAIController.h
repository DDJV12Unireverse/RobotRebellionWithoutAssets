// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CustomAIControllerBase.h"
#include "DroneAIController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(DroneLog, Log, All);

UENUM(BlueprintType)
enum AIDroneState
{
    DRONE_WAITING,
    DRONE_MOVING,
    DRONE_COMBAT,
    DRONE_RECHARGE,

    DRONE_ACTION_COUNT
};

typedef TPair<AIDroneState, float> ActionScore;



/**
 *
 */
UCLASS()
class ROBOTREBELLION_API ADroneAIController : public ACustomAIControllerBase
{
    GENERATED_BODY()


private:
    /************************************************************************/
    /* PROPERTY                                                             */
    /************************************************************************/
    int32 m_alliesAliveCount;
    int32 m_alliesInScene;
    FVector m_groupBarycenter;

    int32 m_ennemyNear;
    int32 m_ennemyInScene;
    FVector m_ennemyNearBarycenter;



    //the height the drone must be
    float m_targetedHeight;

    //the current time
    float m_currentTime;

    float m_debugCooldownDisplayTime;

    bool m_isDebugEnabled;

    //Position to follow
    FVector m_destination;

    //SafeZone
    FVector m_safeZone;

    class AKing* m_king;
    float m_coeffKing;

    void(ADroneAIController::* m_performAction)();
    bool m_actionFinished;
    AIDroneState m_state;
    float m_idleTimer;
    bool m_canDropBomb;


    TArray<class ARobotRebellionCharacter*> m_sensedEnnemies;
    TArray<class ARobotRebellionCharacter*> m_attackZoneCharacters;
    FVector4 m_bestBombLocation;

public:
    /************************************************************************/
    /* UPROPERTY                                                            */
    /************************************************************************/
    //If problem arise (Controller spawns before drone), it will be at this position the drone will be spawned
    //Please, update this according to the real drone position on the map.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGeneral")
        FVector m_defaultDroneSpawnPositionIfProblem;

    //Deceleration Coefficient. Higher the value, faster the drone will arrive to its target and more rough the stop will be.
    //Beware, a too high value will cause instability.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGeneral")
        float m_droneVelocity;

    //Elevation relative to its target
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGeneral")
        float m_stationaryElevation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGeneral")
        float m_detectionDistance;

    /** Max time for reload action*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Time")
        float m_updateSafeZoneCooldownTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "safeZone")
        float m_safeZoneSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "safeZone")
        float m_reloadHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility Theory")
        float m_waitingThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility Theory")
        float m_epsilonSquaredDistanceTolerance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility Theory")
        float m_cooldown;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move")
        class USplineComponent* m_splinePath;

    /** Specify if the path is showed on screen*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DebugParameter)
        bool m_showOriginPath;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DebugParameter)
        bool m_showSmoothedPath;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DebugParameter)
        bool m_showFinalPath;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DebugParameter)
        bool m_showDestination;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 0.f, ClampMax = 1.f))
        float m_splineTension;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 2))
        int32 m_splinePointCountIntraSegment;

    //Noisy travel random value used to modify travel point (to make it more drone like).
    //0.f means no noisy travel method
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 0.f))
        float m_noisyTravelRandom;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 0.f, ClampMax = 3.14159f))
        float m_idleAngle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 0.0001f, ClampMax = 1.f))
        float m_idleAngleSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 0.f))
        float m_idleTranslationSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 0.f))
        float m_idleTranslationGain;

    //acceleration between 0 percent and m_accelPercentPath. Beyond, the drone is at its travel speed.
    //decceleration is the mirror of acceleration. 
    //Thus : 
    //- acceleration between 0% and m_accelPercentPath
    //- travel speed between m_accelPercentPath and 1.f - m_accelPercentPath
    //- decceleration between 1.f - m_accelPercentPath and 100%
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 0.f, ClampMax = 0.5f))
        float m_accelPercentPath;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 0.f))
        float m_idleRotationResetTime;


private:
    TArray<FVector> m_path;
    TArray<FVector> m_smoothedPath;
    TArray<FVector> m_finalPath;

    float m_timeSinceLastUpdate;

    int32 m_currentTripPoint;
    float m_totalTripPoint;

    float m_deccelPercentPath;
    float m_deccelerationCoefficient;

    FVector m_realDroneOrient;
    FVector m_idleForwardGoal;
    FVector m_idleTranslationDirection;
    float m_timeIdleRotationMove;
    


private:
    void internalNoisyTravelTransfertMethod(FVector& inOutPoint, const FVector& nextPoint);

    void internalMakeIdleRotation();

    void internalMakeIdleTranslation();

    void resetTripPoint();


public:
    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/
    ADroneAIController();


    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;

    void updateFrameProperties(float deltaTime);
    
    void updateEnnemiesCampInfo();

    void updateAlliesCampInfo();

    void resetIdleRotationGoal();

    void resetIdleTranslationGoal();

    void saveDroneLocalization();

    EPathFollowingRequestResult::Type stopDroneMoves(class ADrone* drone);

    virtual EPathFollowingRequestResult::Type MoveToTarget() override;

    void makeIdleMove();

    void findDropZone();

    UFUNCTION(BlueprintCallable, Category = "Utility Theory Debug")
        float getAttackScore();

    UFUNCTION(BlueprintCallable, Category = "Utility Theory Debug")
        float getFollowScore();

    UFUNCTION(BlueprintCallable, Category = "Utility Theory Debug")
        float getReloadScore();

    UFUNCTION(BlueprintCallable, Category = "Utility Theory Debug")
        float getWaitingScore();

    UFUNCTION(BlueprintCallable, Category = "Utility Theory Debug")
        float getDropScore();


    bool HasABomb();


    UFUNCTION()
        void receiveBomb();

    UFUNCTION(Reliable, Server, WithValidation)
        void serverReceiveBomb();

    /*Main IA methods*/

    //update the properties of the drone
    void IAUpdate(float deltaTime);

    //The IA Loop
    void IALoop(float deltaTime);


    /*Intermediary IA Methods*/

    void setDestination(const FVector& newDestinationPosition);

    FORCEINLINE bool isArrivedAtDestination() const
    {
        return (m_destination - GetPawn()->GetActorLocation()).SizeSquared() < m_epsilonSquaredDistanceTolerance;
    }

    FORCEINLINE float getTravelCompletionPercentage() const
    {
        return static_cast<float>(m_currentTripPoint) / m_totalTripPoint;
    }

    //update the targeted height of the drone
    void updateTargetedHeight() USE_NOEXCEPT;

    void followKing();

    void followGroup();

    void followFireZone();

    void followSafeZone();

    void waiting();

    void setFollowGroup();

    void setFollowKing();

    void setFollowFireZone();

    void setFollowSafeZone();

    void setWaiting();

    void chooseNextAction();

    void dropBomb();

    virtual void CheckEnnemyNear(float range);
    void CheckEnnemyNearPosition(const FVector& position, float range);


    int getNbBombPlayers();

    float getBombScore(const FVector& position);

    bool isInCombat();

    int getNbEnnemiesInZone(const FVector& zoneCenter);

    FVector findSafeZone();

    FORCEINLINE void enableDroneDisplay(bool enable)
    {
        m_isDebugEnabled = enable;
    }

    FORCEINLINE bool isDebugEnabled()
    {
        return m_isDebugEnabled;
    }

    FORCEINLINE const FVector& getAllyBarycenter() const USE_NOEXCEPT
    {
        return m_groupBarycenter;
    }

    FORCEINLINE const FVector& getEnnemyBarycenter() const USE_NOEXCEPT
    {
        return m_ennemyNearBarycenter;
    }

    void clearSplinePath();

    /*
    Update the spline path using the point array containing all passage point.
    Tension must be between 0.f and 1.f and control the bending strength of the curve.
    */
    void updateSplinePath(float tension);

    // Draw the path on screen
    void debugDrawPath() const;

    void debugElementaryDrawPath(const TArray<FVector>& pathToDraw, const FColor& lineColor) const;

    // Smooth the path, make it more optimal and more realistic
    void smoothPath();

    // See if the agent can go from one point to another
    bool testFlyFromTo(const FVector& startPoint, const FVector& endPoint);

    void processPath();

    bool isLocationFree(const FVector& loc);

    void splineForecast();
};
