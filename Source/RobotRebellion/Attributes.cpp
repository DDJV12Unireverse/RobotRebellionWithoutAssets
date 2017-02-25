// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Attributes.h"


// Sets default values for this component's properties
UAttributes::UAttributes()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

    bReplicates = true;
	// ...

}

void UAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UAttributes, m_health);
    DOREPLIFETIME(UAttributes, m_maxHealth);
    DOREPLIFETIME(UAttributes, m_mana);
    DOREPLIFETIME(UAttributes, m_maxMana);
    DOREPLIFETIME(UAttributes, m_strength);
    DOREPLIFETIME(UAttributes, m_defense);
    DOREPLIFETIME(UAttributes, m_agility);
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

void UAttributes::setMaxMana(float newValue) USE_NOEXCEPT
{
    m_maxMana = newValue;

    if (newValue < m_maxMana)
    {
        m_maxMana = m_maxMana;
    }
}

void UAttributes::setMaxHealth(float newValue) USE_NOEXCEPT
{
    m_maxHealth = newValue;

    if (newValue < m_health)
    {
        m_health = m_maxHealth;
    }
}

void UAttributes::inflictDamage(float damage) USE_NOEXCEPT
{
    if (damage < m_health)
    {
        m_health -= damage;
    }
    else
    {
        m_health = 0;
    }

    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Yellow, TEXT("PV = " + FString::FromInt(m_health));
}