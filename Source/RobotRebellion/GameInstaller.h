// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IsSingleton.h"

#include "IdentifiableObj.h"


/**
 * 
 */
class ROBOTREBELLION_API GameAlterationInstaller : private IsSingleton<GameAlterationInstaller>
{
private:
    GENERATED_USING_FROM_IsSingleton(GameAlterationInstaller);


public:
    TMap<uint32, TSubclassOf<class UAlterationBase>*> m_alterationModelMap;


public:
    template<class AlterationType>
    void installAlteration(TSubclassOf<class UAlterationBase>* alteration)
    {
        m_alterationModelMap.Add(IdentifiableObject<AlterationType>::ID.m_value,
            alteration
        );
    }

    template<class AlterationType>
    TSubclassOf<class UAlterationBase>* getAlterationDefault() const USE_NOEXCEPT
    {
        return m_alterationModelMap[IdentifiableObject<AlterationType>::ID.m_value];
    }
};
