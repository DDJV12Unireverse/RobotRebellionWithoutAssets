// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "WeaponInventory.generated.h"


class UIWeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTREBELLION_API UWeaponInventory : public UActorComponent
{
	GENERATED_BODY()


public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
        UIWeaponBase* m_currentWeapon;


private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
        UIWeaponBase* m_mainWeapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
        UIWeaponBase* m_secondaryWeapon;


public:	
	// Sets default values for this component's properties
	UWeaponInventory();


public:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	
public:
    //change the current equipped weapon to the main weapon
    void changeToMainWeapon() USE_NOEXCEPT;

    //change the current equipped weapon to the secondary weapon
    void changeToSecondaryWeapon() USE_NOEXCEPT;

    //equip the secondary weapon if the current weapon is the main one.
    //equip the main weapon if the current weapon is the secondary one
    void switchWeapon() USE_NOEXCEPT;

    //get the current equipped weapon
    UIWeaponBase* getCurrentWeapon() USE_NOEXCEPT;

    //return true if the current equipped weapon is the main weapon, false otherwise
    bool isMainWeaponEquipped() const USE_NOEXCEPT;

    //return true if the current equipped weapon is the secondary weapon, false otherwise
    bool isSecondaryWeaponEquipped() const USE_NOEXCEPT;
};
