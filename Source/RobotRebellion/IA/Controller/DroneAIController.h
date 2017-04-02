// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CustomAIControllerBase.h"
#include "DroneAIController.generated.h"

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

    //the height the drone must be
    float m_targetedHeight;

    float m_ennemisInScene; // TODO REMPLACER AVEC INFO DE LA SCENE

    //the current time
    float m_currentTime;

    float m_nextDebugDisplayTime;

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

    //King
    UPROPERTY(EditDefaultsOnly, Category = King)
        TSubclassOf<class AKing> m_kingClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move")
        class USplineComponent* m_splinePath;

    /** Specifie if the path is showed on screen*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DebugParameter)
        bool m_showDebugPath;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 0.f, ClampMax = 1.f))
        float m_splineTension;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 2))
        int32 m_splinePointCountIntraSegment;


private:
    TArray<FVector> m_path;
    TArray<FVector> m_smoothedPath;
    TArray<FVector> m_finalPath;

    float m_currentAStarTimer;
    float m_timeSinceLastUpdate;


public:
    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/
    ADroneAIController();


    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;

    float getNbEnnemiesInScene();

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

    //update the player the drone must follows. Temporary for now because the drone will only go follow a living target.
    void updateTargetedTarget();

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

    virtual void CheckEnnemyNear(FVector position, float range);


    int getNbBombPlayers();

    float getBombScore(FVector position);

    bool isInCombat();

    int getNbAliveAllies();

    int getNbAliveEnnemies();

    int getNbEnnemiesInZone(FVector zoneCenter);

    float distance(FVector dest);

    FVector getGroupBarycentre();

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

    void splineForecast();
};
