// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "AlterationBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTREBELLION_API UAlterationBase : public UActorComponent
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

    


public:	
	// Sets default values for this component's properties
	UAlterationBase();

	
    virtual void update(float deltaTime);
    
    virtual void destroyItself();
    
    virtual void onCreate() 
    {}
};
