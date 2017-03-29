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

    bool m_gotBomb = true;

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

    
    //Position to follow
    FVector m_destination;

    //SafeZone
    FVector m_safeZone;

    class AKing* m_king;
    float m_coeffKing;
    void(ADroneAIController::* m_updateTargetMethod)();

    float getAttackScore();

    float getFollowScore();

    float getReloadScore();

    float getWaitingScore();

    float getDropScore();

    float getNbEnnemiesInScene();

    void findDropZone();

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
        float m_updateAttackCooldownTime = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
    float c_bombDamageRadius = 700.0; //TODO move to weapon

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "safeZone")
        float m_safeZoneSize;
    //King
    UPROPERTY(EditDefaultsOnly, Category = King)
        TSubclassOf<class AKing> m_kingClass;
    
    /** Projectile class */
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AProjectile> m_projectileClass;

    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/
    ADroneAIController();


    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;

    virtual EPathFollowingRequestResult::Type MoveToTarget() override;

    bool HasABomb()
    {
        return m_gotBomb;
    }

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

    void CheckEnnemyNear(FVector position, float range);


    int getNbBombPlayers();

    float getBombScore(FVector position);

    bool isInCombat();
 
    int getNbAliveAllies();
    
    int getNbAliveEnnemies();
    
    int getNbEnnemiesInZone(FVector zoneCenter);

    float distance(FVector dest);

    FVector findSafeZone();
};
