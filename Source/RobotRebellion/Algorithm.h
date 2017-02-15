// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * Base Algorithm class
 */
class ROBOTREBELLION_API Algorithm_Base
{
public:
    Algorithm_Base() = default;
    virtual ~Algorithm_Base() = default;


public:
    template<class ... Args>
    void operator()(Args&& ... args);
};


template<class AlgoritmMethod>
class ROBOTREBELLION_API Algorithm final : public Algorithm_Base
{
private:
    AlgoritmMethod m_func;
    

public:
    Algorithm(const AlgoritmMethod& algorithm) :
        m_func{ algorithm }
    {}


public:
    template<class ... Args>
    void operator()(Args&& ... args)
    {
        m_func(std::forward<Args>(args)...);
    }
};


