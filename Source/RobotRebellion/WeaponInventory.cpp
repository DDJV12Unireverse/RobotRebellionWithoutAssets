// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "WeaponInventory.h"


// Sets default values for this component's properties
UWeaponInventory::UWeaponInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UWeaponInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
    changeToMainWeapon();
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
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Main weapon equipped"));
}

void UWeaponInventory::changeToSecondaryWeapon() USE_NOEXCEPT
{
    m_currentWeapon = m_secondaryWeapon;
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Secondary weapon equipped"));
}

UIWeaponBase* UWeaponInventory::getCurrentWeapon() USE_NOEXCEPT
{
    return m_currentWeapon;
}

bool UWeaponInventory::isMainWeaponEquipped() const USE_NOEXCEPT
{
    return m_currentWeapon == m_mainWeapon;
}

bool UWeaponInventory::isSecondaryWeaponEquipped() const USE_NOEXCEPT
{
    return m_currentWeapon == m_secondaryWeapon;
}

void UWeaponInventory::switchWeapon() USE_NOEXCEPT
{
    if (isMainWeaponEquipped())
    {
        changeToSecondaryWeapon();
    }
    else
    {
        changeToMainWeapon();
    }
}
