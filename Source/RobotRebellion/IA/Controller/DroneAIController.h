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


private:
    TArray<FVector> m_path;
    TArray<FVector> m_smoothedPath;
    TArray<FVector> m_finalPath;

    float m_timeSinceLastUpdate;


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

    virtual EPathFollowingRequestResult::Type MoveToTarget() override;

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
