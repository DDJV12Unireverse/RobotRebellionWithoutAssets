// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "IdentifiableObj.h"
#include "AlterationBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTREBELLION_API UAlterationBase : public UActorComponent, public IdentifiableObject<UAlterationBase>
{
	GENERATED_BODY()


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
};
