// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CustomAIControllerBase.h"
#include "DroneAIController.generated.h"

UENUM(BlueprintType)
enum AIDroneState
{
    DRONE_WAITING,
    DRONE_MOVING,
    DRONE_COMBAT
};

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

    AIDroneState m_state;

    //the height the drone must be
    float m_targetedHeight;

    //the current time
    float m_currentTime;

    //the next time we update the drone properties
    float m_nextUpdatePropertyTime;

    //the next time we update the movement of the drone.
    float m_nextMovementUpdateTime;

    
    //Position to follow
    FVector m_destination;

    class AKing* m_king;
    void(ADroneAIController::* m_updateTarget)();

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Time")
        float m_updatePropertyTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Update Time")
        float m_updateMovementTime;

    //King
    UPROPERTY(EditDefaultsOnly, Category = King)
        TSubclassOf<class AKing> m_kingClass;
    

    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/
    ADroneAIController();


    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;

    virtual EPathFollowingRequestResult::Type MoveToTarget() override;


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
    
    void setFollowGroup();
    
    void setFollowKing();
    
};
