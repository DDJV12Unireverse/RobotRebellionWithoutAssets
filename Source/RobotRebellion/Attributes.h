// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class ROBOTREBELLION_API Attributes
{
public:
    using AttributeULongType = unsigned int;
    using AttributeUSmallType = unsigned char;


protected:
    AttributeULongType m_health;
    AttributeULongType m_maxHealth;
    AttributeULongType m_mana;
    AttributeULongType m_maxMana;
    AttributeUSmallType m_strength;
    AttributeUSmallType m_defense;
    AttributeUSmallType m_agility;


protected:
    Attributes()
        : m_health{}, m_maxHealth{}, m_mana{}, m_maxMana{},
        m_strength{}, m_defense{}, m_agility{}
    {}

    Attributes(AttributeULongType health, AttributeULongType maxHealth, AttributeULongType mana, AttributeULongType maxMana,
               AttributeUSmallType strength, AttributeUSmallType defense, AttributeUSmallType agility)
        : m_health{health}, m_maxHealth{maxHealth}, m_mana{mana}, m_maxMana{maxMana},
        m_strength{strength}, m_defense{defense}, m_agility{agility}
    {}
    ~Attributes()
    {}


protected:
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
protected:
    //Inflict damage, reduce the current health value and if damage > health, health goes to 0
    void inflictDamage(AttributeULongType damage) noexcept
    {
        if (damage > m_health)
        {
            m_health = 0;
        }

        m_health -= damage;
    }

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
protected:

};