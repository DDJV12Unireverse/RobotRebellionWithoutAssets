// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Attributes.generated.h"



#define GENERATED_USING_AND_METHODS_FROM_Attributes(attributeName, operator) public: \
using AttributeULongType = UAttributes::AttributeULongType; \
using AttributeUSmallType = UAttributes::AttributeUSmallType; \
AttributeULongType getHealth()    const USE_NOEXCEPT { return attributeName##operator##getHealth(); } \
AttributeULongType getMaxHealth() const USE_NOEXCEPT { return attributeName##operator##getMaxHealth(); } \
AttributeULongType getMana()      const USE_NOEXCEPT { return attributeName##operator##getMana(); } \
AttributeULongType getMaxMana()   const USE_NOEXCEPT { return attributeName##operator##getMaxMana(); } \
AttributeUSmallType getStrength() const USE_NOEXCEPT { return attributeName##operator##getStrength(); } \
AttributeUSmallType getDefense()  const USE_NOEXCEPT { return attributeName##operator##getDefense(); } \
AttributeUSmallType getAgility()  const USE_NOEXCEPT { return attributeName##operator##getAgility(); } \
void setHealth(AttributeULongType newValue)    USE_NOEXCEPT { attributeName##operator##setHealth(newValue); } \
void setMaxHealth(AttributeULongType newValue) USE_NOEXCEPT { attributeName##operator##setMaxHealth(newValue); } \
void setMana(AttributeULongType newValue)      USE_NOEXCEPT { attributeName##operator##setMana(newValue); } \
void setMaxMana(AttributeULongType newValue)   USE_NOEXCEPT { attributeName##operator##setMaxMana(newValue); } \
void setStrength(AttributeUSmallType newValue) USE_NOEXCEPT { attributeName##operator##setStrength(newValue); } \
void setDefense(AttributeUSmallType newValue)  USE_NOEXCEPT { attributeName##operator##setDefense(newValue); } \
void setAgility(AttributeUSmallType newValue)  USE_NOEXCEPT { attributeName##operator##setAgility(newValue); } \
void inflictDamage(AttributeULongType damage)  USE_NOEXCEPT { attributeName##operator##inflictDamage(damage); } \
void restoreHealth(AttributeULongType valueToRestore) USE_NOEXCEPT { attributeName##operator##restoreHealth(valueToRestore); } \
bool isDead() const USE_NOEXCEPT { return attributeName##operator##isDead(); }



UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class ROBOTREBELLION_API UAttributes : public UActorComponent
{
	GENERATED_BODY()


public:
    using AttributeULongType = float;
    using AttributeUSmallType = float;


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
    float m_health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
    float m_maxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
    float m_mana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
    float m_maxMana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
    float m_strength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
    float m_defense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute)
    float m_agility;


public:
	// Sets default values for this component's properties
	UAttributes();


public:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		

public:
    /************************************************************************/
    /*                          GETTER                                      */
    /************************************************************************/
    /*******HEALTH********/
    //get the current health
    AttributeULongType getHealth() const USE_NOEXCEPT
    {
        return m_health;
    }

    //get the maximum health
    AttributeULongType getMaxHealth() const USE_NOEXCEPT
    {
        return m_maxHealth;
    }

    /*******MANA********/
    //get the current mana value
    AttributeULongType getMana() const USE_NOEXCEPT
    {
        return m_mana;
    }

    //get the maximum mana value
    AttributeULongType getMaxMana() const USE_NOEXCEPT
    {
        return m_maxMana;
    }

    /*******STRENGTH********/
    //get the current strength 
    AttributeUSmallType getStrength() const USE_NOEXCEPT
    {
        return m_strength;
    }

    /*******DEFENSE********/
    // get the current defense
    AttributeUSmallType getDefense() const USE_NOEXCEPT
    {
        return m_defense;
    }

    /*******AGILITY********/
    // get the current agility
    AttributeUSmallType getAgility() const USE_NOEXCEPT
    {
        return m_agility;
    }

    /************************************************************************/
    /*                          SETTER                                      */
    /************************************************************************/
    /*******HEALTH********/
    //set the current health
    void setHealth(AttributeULongType newValue) USE_NOEXCEPT
    {
        m_health = (newValue > m_maxHealth) ? m_maxHealth : newValue;
    }

    //set the maximum value of health
    void setMaxHealth(AttributeULongType newValue) USE_NOEXCEPT;

    /*******MANA********/
    // set the current mana value
    void setMana(AttributeULongType newValue) USE_NOEXCEPT
    {
        m_mana = (newValue > m_maxMana) ? m_maxMana : newValue;
    }

    //set the maximum mana value
    void setMaxMana(AttributeULongType newValue) USE_NOEXCEPT;

    /*******STRENGTH********/
    // set the current strength value
    void setStrength(AttributeUSmallType newValue) USE_NOEXCEPT
    {
        m_strength = newValue;
    }

    /*******DEFENSE********/
    // set the current defense value
    void setDefense(AttributeUSmallType newValue) USE_NOEXCEPT
    {
        m_defense = newValue;
    }

    /*******AGILITY********/
    // set the current agility value
    void setAgility(AttributeUSmallType newValue) USE_NOEXCEPT
    {
        m_agility = newValue;
    }


    /************************************************************************/
    /*                          UTILITARY                                   */
    /************************************************************************/
public:
    //Inflict damage, reduce the current health value and if damage > health, health goes to 0
    void inflictDamage(AttributeULongType damage) USE_NOEXCEPT;

    //restore current health value and if the value to restore is over max_health, health goes to max_health
    void restoreHealth(AttributeULongType valueToRestore) USE_NOEXCEPT
    {
        setHealth(m_health + valueToRestore);
    }

    //return true if the player's current health is 0, false otherwise.
    bool isDead() const USE_NOEXCEPT
    {
        return m_health == 0;
    }


    //Déserialization and serialization methods
public:

};
