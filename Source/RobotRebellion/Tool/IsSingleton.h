// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#define GENERATED_USING_FROM_IsSingleton(ObjectClassName) private: \
	friend class IsSingleton<ObjectClassName>; \
public: \
    using IsSingleton<ObjectClassName>::getInstance; \



/**
 * 
 */
template<class Object>
class ROBOTREBELLION_API IsSingleton
{
private:
    static Object m_instance;


protected:
    IsSingleton() = default;


public:
    virtual ~IsSingleton() = default;


private:
    IsSingleton(IsSingleton&) = delete;
    IsSingleton& operator=(IsSingleton&) = delete;


protected:
    static Object& getInstance() noexcept
    {
        return m_instance;
    }
};

template<class Object> Object IsSingleton<Object>::m_instance;