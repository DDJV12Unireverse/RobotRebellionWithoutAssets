// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "WeaponInventory.h"


// Sets default values for this component's properties
UWeaponInventory::UWeaponInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponInventory::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UWeaponInventory::changeToMainWeapon() USE_NOEXCEPT
{
    m_currentWeapon = m_mainWeapon;
}

void UWeaponInventory::changeToSecondaryWeapon() USE_NOEXCEPT
{
    m_currentWeapon = m_secondaryWeapon;
}

UIWeaponBase* UWeaponInventory::getCurrentWeapon() USE_NOEXCEPT
{
    return m_currentWeapon;
}