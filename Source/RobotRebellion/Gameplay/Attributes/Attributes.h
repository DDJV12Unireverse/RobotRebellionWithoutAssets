// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Attributes.generated.h"



#define GENERATED_USING_AND_METHODS_FROM_Attributes(attributeName, operator) public: \
float getHealth()    const USE_NOEXCEPT { return attributeName##operator##getHealth(); } \
float getMaxHealth() const USE_NOEXCEPT { return attributeName##operator##getMaxHealth(); } \
float getMana()      const USE_NOEXCEPT { return attributeName##operator##getMana(); } \
float getMaxMana()   const USE_NOEXCEPT { return attributeName##operator##getMaxMana(); } \
float getStrength()  const USE_NOEXCEPT { return attributeName##operator##getStrength(); } \
float getDefense()   const USE_NOEXCEPT { return attributeName##operator##getDefense(); } \
float getAgility()   const USE_NOEXCEPT { return attributeName##operator##getAgility(); } \
float getShield()   const USE_NOEXCEPT { return attributeName##operator##getShield(); } \
void setHealth(float newValue)    USE_NOEXCEPT { attributeName##operator##setHealth(newValue); } \
void setMaxHealth(float newValue) USE_NOEXCEPT { attributeName##operator##setMaxHealth(newValue); } \
void setMana(float newValue)      USE_NOEXCEPT { attributeName##operator##setMana(newValue); } \
void setMaxMana(float newValue)   USE_NOEXCEPT { attributeName##operator##setMaxMana(newValue); } \
void setStrength(float newValue) USE_NOEXCEPT { attributeName##operator##setStrength(newValue); } \
void setDefense(float newValue)  USE_NOEXCEPT { attributeName##operator##setDefense(newValue); } \
void setAgility(float newValue)  USE_NOEXCEPT { attributeName##operator##setAgility(newValue); } \
void removeShield(float newValue) USE_NOEXCEPT { attributeName##operator##removeShield(newValue); } \
bool isDead() const USE_NOEXCEPT { return !attributeName##operator##isImmortal() && attributeName##operator##isDead(); } \
void setImmortal(bool isImmortal) const USE_NOEXCEPT { attributeName##operator##setImmortal(isImmortal); } \
bool isImmortal() const USE_NOEXCEPT { return attributeName##operator##isImmortal(); } \
void consumeMana(float manaAmount) USE_NOEXCEPT { return attributeName##operator##consumeMana(manaAmount); }



UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class ROBOTREBELLION_API UAttributes : public UActorComponent
{
    GENERATED_BODY()


protected:
    /************************************************************************/
    /* UPROPERTY                                                            */
    /************************************************************************/


    UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Attribute)
        float m_health;

    UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Attribute)
        float m_maxHealth;

    UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Attribute)
        float m_mana;

    UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Attribute)
        float m_maxMana;

    UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Attribute)
        float m_strength;

    UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Attribute)
        float m_defense;

    UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Attribute)
        float m_agility;

    UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Attribute)
        float m_shield;



    /************************************************************************/
    /* PROPERTY                                                             */
    /************************************************************************/


    void(UAttributes::*m_inflictDamageDelegate)(float);
    void(UAttributes::*m_restoreHealthDelegate)(float);
    void(UAttributes::*m_restoreManaDelegate)(float);


public:
    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/


    // Sets default values for this component's properties
    UAttributes();



    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


    /************************************************************************/
    /*                          GETTER                                      */
    /************************************************************************/
    /*******HEALTH********/
    //get the current health
    float getHealth() const USE_NOEXCEPT
    {
        return m_health;
    }

    //get the maximum health
    float getMaxHealth() const USE_NOEXCEPT
    {
        return m_maxHealth;
    }

    /*******MANA********/
    //get the current mana value
    float getMana() const USE_NOEXCEPT
    {
        return m_mana;
    }

    //get the maximum mana value
    float getMaxMana() const USE_NOEXCEPT
    {
        return m_maxMana;
    }

    /*******STRENGTH********/
    //get the current strength 
    float getStrength() const USE_NOEXCEPT
    {
        return m_strength;
    }

    /*******DEFENSE********/
    // get the current defense
    float getDefense() const USE_NOEXCEPT
    {
        return m_defense;
    }

    /*******AGILITY********/
    // get the current agility
    float getAgility() const USE_NOEXCEPT
    {
        return m_agility;
    }

    /*******SHIELD********/
    // get the current shield
    float getShield() const USE_NOEXCEPT
    {
        return m_shield;
    }

    /************************************************************************/
    /*                          SETTER                                      */
    /************************************************************************/
    /*******HEALTH********/
    //set the current health
    void setHealth(float newValue) USE_NOEXCEPT
    {
        m_health = (newValue > m_maxHealth) ? m_maxHealth : ((newValue < 0.f) ? 0.f : newValue);
    }

    //set the maximum value of health
    void setMaxHealth(float newValue) USE_NOEXCEPT;

    /*******MANA********/
    // set the current mana value
    void setMana(float newValue) USE_NOEXCEPT
    {
        m_mana = (newValue > m_maxMana) ? m_maxMana : ((newValue < 0.f) ? 0.f : newValue);
    }

    //set the maximum mana value
    void setMaxMana(float newValue) USE_NOEXCEPT;

    /*******STRENGTH********/
    // set the current strength value
    void setStrength(float newValue) USE_NOEXCEPT
    {
        m_strength = newValue;
    }

    /*******DEFENSE********/
    // set the current defense value
    void setDefense(float newValue) USE_NOEXCEPT
    {
        m_defense = newValue;
    }

    /*******AGILITY********/
    // set the current agility value
    void setAgility(float newValue) USE_NOEXCEPT
    {
        m_agility = newValue;
    }

    /*******SHIELD********/
    // set the current shield value
    void setShield(float newValue) USE_NOEXCEPT
    {
        m_shield = newValue;
    }


    /************************************************************************/
    /*                          UTILITARY                                   */
    /************************************************************************/

    //Inflict damage, reduce the current health value and if damage > health, health goes to 0
    void inflictDamage(float damage)
    {
        (this->*m_inflictDamageDelegate)(damage);
    }

    void consumeMana(float manaAmount);

    //restore current mana value and if the value to restore is over max_mana, mana goes to max_mana
    void restoreMana(float valueToRestore)
    {
        (this->*m_restoreManaDelegate)(valueToRestore);
    }

    //restore current health value and if the value to restore is over max_health, health goes to max_health
    void restoreHealth(float valueToRestore)
    {
        (this->*m_restoreHealthDelegate)(valueToRestore);
    }

    //return true if the player's current health is 0, false otherwise.
    bool isDead() const USE_NOEXCEPT
    {
        return m_health == 0;
    }

    void setImmortal(bool isImmortal) USE_NOEXCEPT;

    bool isImmortal() const USE_NOEXCEPT;

    // Add some shield (Do not use negative value, use removeShield)
    void addShield(float amount)
    {
        m_shield += amount;
    }

    // Remove some shield
    void removeShield(float amount);


private:
    void inflictDamageMortal(float damage);

    void restoreHealthMortal(float restoreValue)
    {
        setHealth(m_health + restoreValue);
    }

    void restoreManaMortal(float restoreValue)
    {
        setMana(m_mana + restoreValue);
    }

    void immortalMethod(float)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Magenta, "IMMORTAL OBJECT");
    }
};
