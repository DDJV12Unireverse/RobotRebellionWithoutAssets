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
    void changeToMainWeapon() USE_NOEXCEPT;

    void changeToSecondaryWeapon() USE_NOEXCEPT;

    UIWeaponBase* getCurrentWeapon() USE_NOEXCEPT;

    bool isMainWeaponEquipped() const USE_NOEXCEPT;

    bool isSecondaryWeaponEquipped() const USE_NOEXCEPT;
};
