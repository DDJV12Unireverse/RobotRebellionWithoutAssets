// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tool/IsSingleton.h"
/**
 * 
 */
class ROBOTREBELLION_API EntityDataSingleton : private IsSingleton<EntityDataSingleton>
{
    GENERATED_USING_FROM_IsSingleton(EntityDataSingleton)


public:
    TArray<class ARobotsCharacter*> m_robotArray;
    TArray<class ASovecCharacter*> m_sovecArray;
    TArray<class ABeastCharacter*> m_beastArray;
    TArray<class APlayableCharacter*> m_playableCharacterArray;

    class AKing* m_king;
    class ADrone* m_drone;

    int8 m_showVolumeBox : 1;
    int8 m_showEnnemyDetectionSphere : 1;

    int8 fill : 6;



public:
    void update(const class UWorld* world);


private:
    template<class Base, class GenericType>
    bool updateType(Base* current, GenericType*& typeToUpdate)
    {
        GenericType* temp = Cast<GenericType>(current);
        if (temp)
        {
            typeToUpdate = temp;
            return true;
        }
        return false;
    }

    template<class Base, class GenericType>
    bool updateType(Base* current, TArray<GenericType*>& typeToUpdate)
    {
        GenericType* temp = Cast<GenericType>(current);
        if(temp)
        {
            typeToUpdate.Emplace(temp);
            return true;
        }
        return false;
    }
};