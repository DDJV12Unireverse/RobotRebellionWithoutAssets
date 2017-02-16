// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Attributes.generated.h"


#define GENERATED_USING_AND_METHODS_FROM_Attributes(attributeName, operator) public: \
using AttributeULongType = UAttributes::AttributeULongType; \
using AttributeUSmallType = UAttributes::AttributeUSmallType; \
AttributeULongType getHealth()    const noexcept { return attributeName##operator##getHealth(); } \
AttributeULongType getMaxHealth() const noexcept { return attributeName##operator##getMaxHealth(); } \
AttributeULongType getMana()      const noexcept { return attributeName##operator##getMana(); } \
AttributeULongType getMaxMana()   const noexcept { return attributeName##operator##getMaxMana(); } \
AttributeUSmallType getStrength() const noexcept { return attributeName##operator##getStrength(); } \
AttributeUSmallType getDefense()  const noexcept { return attributeName##operator##getDefense(); } \
AttributeUSmallType getAgility()  const noexcept { return attributeName##operator##getAgility(); } \
void setHealth(AttributeULongType newValue)    noexcept { attributeName##operator##setHealth(newValue); } \
void setMaxHealth(AttributeULongType newValue) noexcept { attributeName##operator##setMaxHealth(newValue); } \
void setMana(AttributeULongType newValue)      noexcept { attributeName##operator##setMana(newValue); } \
void setMaxMana(AttributeULongType newValue)   noexcept { attributeName##operator##setMaxMana(newValue); } \
void setStrength(AttributeUSmallType newValue) noexcept { attributeName##operator##setStrength(newValue); } \
void setDefense(AttributeUSmallType newValue)  noexcept { attributeName##operator##setDefense(newValue); } \
void setAgility(AttributeUSmallType newValue)  noexcept { attributeName##operator##setAgility(newValue); } \
void inflictDamage(AttributeULongType damage)  noexcept { attributeName##operator##inflictDamage(damage); } \
void restoreHealth(AttributeULongType valueToRestore) noexcept { attributeName##operator##restoreHealth(valueToRestore); } \
bool isDead() const noexcept { return attributeName##operator##isDead(); }


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
    AttributeULongType getHealth() const noexcept
    {
        return m_health;
    }

    //get the maximum health
    AttributeULongType getMaxHealth() const noexcept
    {
        return m_maxHealth;
    }

    /*******MANA********/
    //get the current mana value
    AttributeULongType getMana() const noexcept
    {
        return m_mana;
    }

    //get the maximum mana value
    AttributeULongType getMaxMana() const noexcept
    {
        return m_maxMana;
    }

    /*******STRENGTH********/
    //get the current strength 
    AttributeUSmallType getStrength() const noexcept
    {
        return m_strength;
    }

    /*******DEFENSE********/
    // get the current defense
    AttributeUSmallType getDefense() const noexcept
    {
        return m_defense;
    }

    /*******AGILITY********/
    // get the current agility
    AttributeUSmallType getAgility() const noexcept
    {
        return m_agility;
    }

    /************************************************************************/
    /*                          SETTER                                      */
    /************************************************************************/
    /*******HEALTH********/
    //set the current health
    void setHealth(AttributeULongType newValue) noexcept
    {
        m_health = (newValue > m_maxHealth) ? m_maxHealth : newValue;
    }

    //set the maximum value of health
    void setMaxHealth(AttributeULongType newValue) noexcept;

    /*******MANA********/
    // set the current mana value
    void setMana(AttributeULongType newValue) noexcept
    {
        m_mana = (newValue > m_maxMana) ? m_maxMana : newValue;
    }

    //set the maximum mana value
    void setMaxMana(AttributeULongType newValue) noexcept;

    /*******STRENGTH********/
    // set the current strength value
    void setStrength(AttributeUSmallType newValue) noexcept
    {
        m_strength = newValue;
    }

    /*******DEFENSE********/
    // set the current defense value
    void setDefense(AttributeUSmallType newValue) noexcept
    {
        m_defense = newValue;
    }

    /*******AGILITY********/
    // set the current agility value
    void setAgility(AttributeUSmallType newValue) noexcept
    {
        m_agility = newValue;
    }


    /************************************************************************/
    /*                          UTILITARY                                   */
    /************************************************************************/
public:
    //Inflict damage, reduce the current health value and if damage > health, health goes to 0
    void inflictDamage(AttributeULongType damage) noexcept;

    //restore current health value and if the value to restore is over max_health, health goes to max_health
    void restoreHealth(AttributeULongType valueToRestore) noexcept
    {
        setHealth(m_health + valueToRestore);
    }

    //return true if the player's current health is 0, false otherwise.
    bool isDead() const noexcept
    {
        return m_health == 0;
    }


    //Déserialization and serialization methods
public:

};
