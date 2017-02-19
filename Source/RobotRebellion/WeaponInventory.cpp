// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "WeaponInventory.h"
#include "IWeaponBase.h"

#include "UtilitaryMacros.h"

// Sets default values for this component's properties
UWeaponInventory::UWeaponInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

    //m_mainWeapon = CreateDefaultSubobject<ULongRangeWeapon>(TEXT("mainWeapon"));
    //m_secondaryWeapon = CreateDefaultSubobject<UShortRangeWeapon>(TEXT("secondaryWeapon"));

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
    m_currentWeapon = m_mainWeapon.GetDefaultObject();
    PRINT_MESSAGE_ON_SCREEN(FColor::Blue, TEXT("Main weapon equipped"));
}

void UWeaponInventory::changeToSecondaryWeapon() USE_NOEXCEPT
{
    m_currentWeapon = m_secondaryWeapon.GetDefaultObject();
    PRINT_MESSAGE_ON_SCREEN(FColor::Blue, TEXT("Secondary weapon equipped"));
}

UIWeaponBase* UWeaponInventory::getCurrentWeapon() USE_NOEXCEPT
{
    return m_currentWeapon;
}

bool UWeaponInventory::isMainWeaponEquipped() const USE_NOEXCEPT
{
    return m_currentWeapon == m_mainWeapon.GetDefaultObject();
}

bool UWeaponInventory::isSecondaryWeaponEquipped() const USE_NOEXCEPT
{
    return m_currentWeapon == m_secondaryWeapon.GetDefaultObject();
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

FString UWeaponInventory::toFString() const USE_NOEXCEPT
{
    FString message(TEXT("Current weapon : "));
    if (isMainWeaponEquipped())
    {
        return message + TEXT("Main Weapon");
    }
    else
    {
        return message + TEXT("Secondary Weapon");
    }
}
