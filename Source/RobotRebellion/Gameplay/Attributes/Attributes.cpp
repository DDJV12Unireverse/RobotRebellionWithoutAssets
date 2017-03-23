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

    setImmortal(false);
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
void UAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UAttributes::setMaxMana(float newValue) USE_NOEXCEPT
{
    m_maxMana = newValue;

    if(m_maxMana < m_mana)
    {
        m_mana = m_maxMana;
    }
}

void UAttributes::setMaxHealth(float newValue) USE_NOEXCEPT
{
    m_maxHealth = newValue;

    if(m_maxHealth < m_health)
    {
        m_health = m_maxHealth;
    }
}

void UAttributes::inflictDamageMortal(float damage)
{
    if(damage < m_health)
    {
        m_health -= damage;
    }
    else
    {
        m_health = 0;
    }

    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Yellow, TEXT("PV = " + FString::FromInt(m_health)));
}

void UAttributes::consumeMana(float manaAmount)
{
    if(manaAmount < m_mana)
    {
        m_mana -= manaAmount;
    }
    else
    {
        m_mana = 0;
    }
}

void UAttributes::setImmortal(bool isImmortal) USE_NOEXCEPT
{
    if(isImmortal)
    {
        m_inflictDamageDelegate = &UAttributes::immortalMethod;
        m_restoreHealthDelegate = &UAttributes::immortalMethod;
        m_restoreManaDelegate = &UAttributes::immortalMethod;
    }
    else
    {
        m_inflictDamageDelegate = &UAttributes::inflictDamageMortal;
        m_restoreHealthDelegate = &UAttributes::restoreHealthMortal;
        m_restoreManaDelegate = &UAttributes::restoreManaMortal;
    }
}

bool UAttributes::isImmortal() const USE_NOEXCEPT
{
    return m_inflictDamageDelegate == &UAttributes::immortalMethod;
}
