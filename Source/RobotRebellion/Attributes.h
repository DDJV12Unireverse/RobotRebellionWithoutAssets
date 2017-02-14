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

    /*
    * GETTER
    */
    // HEALTH
    unsigned int getHealth() const noexcept
    {
        return m_health;
    }
    unsigned int getMaxHealth() const noexcept
    {
        return m_maxHealth;
    }
    // MANA
    unsigned int getMana() const noexcept
    {
        return m_mana;
    }
    unsigned int getMaxMana() const noexcept
    {
        return m_maxMana;
    }
    // STRENGTH
    unsigned char getStrength() const noexcept
    {
        return m_strength;
    }
    // DEFENSE
    unsigned char getDefense() const noexcept
    {
        return m_defense;
    }
    // AGILITY
    unsigned char getAgility() const noexcept
    {
        return m_agility;
    }

    /*
    * SETTER
    */
};