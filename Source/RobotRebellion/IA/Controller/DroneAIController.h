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
    DRONE_BOMB,
    DRONE_RECHARGE,
    DRONE_NULL // Error
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


    TMap<AIDroneState,float> m_scores;

  //  bool m_gotBomb = false;

    AIDroneState m_state;

    //the height the drone must be
    float m_targetedHeight;

    //the current time
    float m_currentTime;

    //the next time we update the drone properties
    float m_nextUpdatePropertyTime;

    //the next time we update the movement of the drone.
    float m_nextMovementUpdateTime;

    //the next time the drone checks we are attacking or attacked by ennemies
    float m_nextUpdateAttackCooldownTime;

    //the next time the drone checks we are attacking or attacked by ennemies
    float m_nextDebugDisplayTime;

    bool m_isDebugEnabled;
    
    //Position to follow
    FVector m_destination;

    //SafeZone
    FVector m_safeZone;

    class AKing* m_king;
    float m_coeffKing;


    void(ADroneAIController::* m_updateTargetMethod)();


    TArray<class ARobotRebellionCharacter *> m_sensedEnnemies;
    TArray<class ARobotRebellionCharacter *> m_attackZoneCharacters;
    FVector4 m_bestBombLocation;

public:
    /************************************************************************/
    /* UPROPERTY                                                            */
    /************************************************************************/

    //Deceleration Coefficient. Higher the value, faster the drone will arrive to its target and more rough the stop will be.
    //Beware, a too high value will cause instability.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGeneral")
        float m_decelerationCoefficient;

    //Elevation relative to its target
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGeneral")
        float m_stationaryElevation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGeneral")
        float m_detectionDistance = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Time")
        float m_updatePropertyTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Time")
        float m_updateMovementTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Time")
        float m_updateAttackCooldownTime = 2.25f;

   // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
    //float c_bombDamageRadius = 700.0; //TODO move to weapon

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "safeZone")
        float m_safeZoneSize;
    //King
    UPROPERTY(EditDefaultsOnly, Category = King)
        TSubclassOf<class AKing> m_kingClass;
    
    /** Projectile class */
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AProjectile> m_projectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move")
        class USplineComponent* m_splinePath;

    /** Specifie how many time aicontroller wait before updating everything (in ms)*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UpdateParameter)
        float m_updateFrequencie;
    /** Specifie if the path is showed on screen*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DebugParameter)
        bool m_showDebugPath;
    /** Specifie the time Step along the spline curve. Must be between 0 and 1. It's more like a percentage*/
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DebugParameter, meta = (ClampMin = 0.f, ClampMax = 1.f))
        float m_timeStep;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move", meta = (ClampMin = 0.f, ClampMax = 1.f))
        float m_splineTension;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Move")
        float m_timerAStarProcess;


private:
    TArray<FVector> m_path;
    TArray<FVector> m_smoothedPath;

    //Debug
    int32 m_targetId;
    int32 m_oldTarget;

    float m_time;

    float m_currentAStarTimer;


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

    //update the targeted height of the drone
    void updateTargetedHeight() USE_NOEXCEPT;

    //update the player the drone must follows. Temporary for now because the drone will only go follow a living target.
    void updateTargetedTarget();

    void followKing();
    
    void followGroup();

    void followFireZone();

    void followSafeZone();
    
    void setFollowGroup();
    
    void setFollowKing();

    void setFollowFireZone();

    void setFollowSafeZone();
    
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

    FVector findSafeZone();

    void enableDroneDisplay(bool enable)
    {
        m_isDebugEnabled = enable;
    }
    bool isDebugEnabled()
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
    void debugDrawPath();

    // Smooth the path, make it more optimal and more realistic
    void smoothPath();

    // See if the agent can go from one point to another
    bool testFlyFromTo(const FVector& startPoint, const FVector& endPoint);

    void processPath(float deltaTime);
};
