// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class ROBOTREBELLION_API Identifiable
{
public:
    struct ID
    {
    private:
        static uint32 accumulator;


    public:
        uint32 m_value;


    public:
        ID() USE_NOEXCEPT : 
            m_value{ accumulator++ }
        {}

        ID(uint32 newId) USE_NOEXCEPT : 
            m_value{ newId }
        {}
    };


public:
    Identifiable::ID m_id;


    Identifiable(const Identifiable::ID& id) USE_NOEXCEPT :
        m_id{ id }
    {}

	virtual ~Identifiable() 
    {}
};


template<class ObjectType>
class ROBOTREBELLION_API IdentifiableObject : public Identifiable
{
public:
    static Identifiable::ID ID;


public:
    IdentifiableObject() USE_NOEXCEPT : 
        Identifiable{ IdentifiableObject<ObjectType>::ID }
    {}

    virtual ~IdentifiableObject() {}
};

template<class ObjectType> Identifiable::ID IdentifiableObject<ObjectType>::ID;


