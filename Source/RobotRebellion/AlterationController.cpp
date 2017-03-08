// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "AlterationController.h"

#include "AlterationBase.h"

#include "RobotRebellionCharacter.h"

#include "UtilitaryFunctionLibrary.h"

// Sets default values for this component's properties
UAlterationController::UAlterationController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAlterationController::BeginPlay()
{
	Super::BeginPlay();

	// ...
    
    UUtilitaryFunctionLibrary::bindServerClientMethodPtr(
        GetOwner(),
        m_updateMethod,
        &UAlterationController::update,
        &UAlterationController::doesNothing
    );

    UUtilitaryFunctionLibrary::bindServerClientMethodPtr(
        GetOwner(),
        m_inflictMethod,
        &UAlterationController::addAlterationServerImp,
        &UAlterationController::addAlterationClientImp
    );
}


// Called every frame
void UAlterationController::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

    (this->*m_updateMethod)(DeltaTime);
}

void UAlterationController::update(float deltaTime)
{
    m_alterationsArray.RemoveAll([](UAlterationBase* current) { return current->IsPendingKillOrUnreachable(); });

    for (int32 iter = 0; iter < m_alterationsArray.Num(); ++iter)
    {
        if (!m_alterationsArray[iter]->IsPendingKillOrUnreachable())
        {
            m_alterationsArray[iter]->update(deltaTime);
        }
        else
        {
            m_alterationsArray.RemoveAt(iter);
            --iter;
        }
    }

    m_alterationsArray.RemoveAll([](UAlterationBase* current) { return current->IsPendingKillOrUnreachable(); });
}

UAlterationBase** UAlterationController::findByID(int32 id)
{
    for (auto iter = 0; iter < m_alterationsArray.Num(); ++iter)
    {
        if (m_alterationsArray[iter]->getID().m_value == id)
        {
            return &m_alterationsArray[iter];
        }
    }

    return nullptr;
}

void UAlterationController::removeAllAlteration()
{
    while (m_alterationsArray.Num() != 0)
    {
        UAlterationBase* alterationToDestroy = m_alterationsArray.Pop(false);
        alterationToDestroy->destroyItself();
    }
}


GENERATE_DECLARATION_SERVER_CLIENT_METHODS_BASED_VALIDATION_SERVER_FROM_METHOD_PTR_WITH_CLIENT_IMPL_GEN(m_inflictMethod, UAlterationController, addAlteration, serverAddAlteration, UAlterationBase*, alteredOwner);


void UAlterationController::addAlterationServerImp(UAlterationBase* newAlteration)
{
    if (newAlteration)
    {
        if (!this->findByID(newAlteration->getID().m_value))
        {
            m_alterationsArray.Add(newAlteration);
            newAlteration->onCreate(Cast<ARobotRebellionCharacter>(GetOwner()));

            auto character = Cast<ARobotRebellionCharacter>(GetOwner());
            if (character)
            {
                character->displayAnimatedText(newAlteration->toDebugString(), FColor::Silver, ELivingTextAnimMode::TEXT_ANIM_BOING_BOING);
            }
        }
    }
}