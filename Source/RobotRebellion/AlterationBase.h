// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "AlterationBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTREBELLION_API UAlterationBase : public UActorComponent
{
	GENERATED_BODY()


public:
    struct ID
    {
    public:
        int32 m_value;

        static int32 attributor;

        ID() USE_NOEXCEPT : m_value{ attributor++ } {}
    };


public:
    /************************************************************************/
    /* UPROPERTY                                                            */
    /************************************************************************/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
        float m_lifeTime;




    /************************************************************************/
    /* PROPERTY                                                             */
    /************************************************************************/
    float m_currentTime;

    class ARobotRebellionCharacter* m_alteredOwner;


private:
    static ID m_id;


public:	
	// Sets default values for this component's properties
	UAlterationBase();

	
    virtual void update(float deltaTime);
    
    virtual void destroyItself();
    
    virtual void onCreate(class ARobotRebellionCharacter* alteredOwner)
    {
        m_alteredOwner = alteredOwner;
    }

    virtual FString toDebugString() const USE_NOEXCEPT
    {
        return "Base Alteration";
    }

    //No, it isn't the way Patrice taught us but I don't have the time luxury to implement it the correct way.
    virtual ID getID() const USE_NOEXCEPT
    {
        return UAlterationBase::m_id;
    }
};
