// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Attributes.h"


// Sets default values for this component's properties
UAttributes::UAttributes()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

}


// Called when the game starts
void UAttributes::BeginPlay()
{
	Super::BeginPlay();

    

	// ...
	
}


// Called every frame
void UAttributes::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UAttributes::setMaxMana(AttributeULongType newValue) noexcept
{
    m_maxMana = newValue;

    if (newValue < m_maxMana)
    {
        m_maxMana = m_maxMana;
    }
}

void UAttributes::setMaxHealth(AttributeULongType newValue) noexcept
{
    m_maxHealth = newValue;

    if (newValue < m_health)
    {
        m_health = m_maxHealth;
    }
}

void UAttributes::inflictDamage(AttributeULongType damage) noexcept
{
    if (damage < m_health)
    {
        m_health -= damage;
    }
    else
    {
        m_health = 0;
    }
}