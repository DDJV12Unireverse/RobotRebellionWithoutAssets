// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class ROBOTREBELLION_API Attributes
{
protected:
    unsigned int m_health;
    unsigned int m_maxHealth;
    unsigned int m_mana;
    unsigned int m_maxMana;
    unsigned char m_strength;
    unsigned char m_defense;
    unsigned char m_agility;


protected:
    Attributes()
        : m_health{}, m_maxHealth{}, m_mana{}, m_maxMana{},
        m_strength{}, m_defense{}, m_agility{}
    {}

    Attributes(unsigned int health, unsigned int maxHealth, unsigned int mana, unsigned int maxMana,
               unsigned char strength, unsigned char defense, unsigned char agility)
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
    unsigned int getHealth() const noexcept
    {
        return m_health;
    }

    //get the maximum health
    unsigned int getMaxHealth() const noexcept
    {
        return m_maxHealth;
    }

    /*******MANA********/
    //get the current mana value
    unsigned int getMana() const noexcept
    {
        return m_mana;
    }

    //get the maximum mana value
    unsigned int getMaxMana() const noexcept
    {
        return m_maxMana;
    }

    /*******STRENGTH********/
    //get the current strength 
    unsigned char getStrength() const noexcept
    {
        return m_strength;
    }

    /*******DEFENSE********/
    // get the current defense
    unsigned char getDefense() const noexcept
    {
        return m_defense;
    }

    /*******AGILITY********/
    // get the current agility
    unsigned char getAgility() const noexcept
    {
        return m_agility;
    }

    /************************************************************************/
    /*                          SETTER                                      */
    /************************************************************************/
    /*******HEALTH********/
    //set the current health
    unsigned int setHealth(unsigned int newValue) noexcept
    {
        m_health = (newValue > m_maxHealth) ? m_maxHealth : newValue;
    }

    //set the maximum value of health
    unsigned int setMaxHealth(unsigned int newValue) noexcept;

    /*******MANA********/
    // set the current mana value
    unsigned int setMana(unsigned int newValue) noexcept
    {
        m_mana = (newValue > m_maxMana) ? m_maxMana : newValue;
    }

    //set the maximum mana value
    unsigned int setMaxMana(unsigned int newValue) noexcept;

    /*******STRENGTH********/
    // set the current strength value
    unsigned char setStrength(unsigned char newValue) noexcept
    {
        m_strength = newValue;
    }

    /*******DEFENSE********/
    // set the current defense value
    unsigned char setDefense(unsigned char newValue) noexcept
    {
        m_defense = newValue;
    }

    /*******AGILITY********/
    // set the current agility value
    unsigned char setAgility(unsigned char newValue) noexcept
    {
        m_agility = newValue;
    }


    /************************************************************************/
    /*                          UTILITARY                                   */
    /************************************************************************/
protected:

};