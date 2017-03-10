// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "AlterationController.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTREBELLION_API UAlterationController : public UActorComponent
{
	GENERATED_BODY()


public:
    /************************************************************************/
    /* PROPERTY                                                             */
    /************************************************************************/
    TArray<class UAlterationBase*> m_alterationsArray;

    void (UAlterationController::* m_updateMethod)(float);


public:	
    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

	// Sets default values for this component's properties
	UAlterationController();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    class UAlterationBase** findByID(int32 id);

    void removeAllAlteration();


private:
    void doesNothing(float) {}
    void update(float deltaTime);



public:
    /************************************************************************/
    /* UFUNCTION                                                            */
    /************************************************************************/
    UFUNCTION(BlueprintCallable, Category = "Alteration")
        void addAlteration(class UAlterationBase* newAlteration);


    UFUNCTION(Reliable, Server, WithValidation)
        void serverAddAlteration(class UAlterationBase* newAlteration);

    GENERATE_PROTOTYPE_SERVER_CLIENT_METHODS_BASED_VALIDATION_SERVER(m_inflictMethod, UAlterationController, addAlteration, class UAlterationBase*);
};
