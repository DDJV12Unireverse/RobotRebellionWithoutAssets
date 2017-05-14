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
    TArray<class APlayableCharacter*> m_playableCharacterArray;

    class AKing* m_king;
    class ADrone* m_drone;

    int8 m_showVolumeBox : 1;
    int8 m_showEnnemyDetectionSphere : 1;

    int8 fill : 6;


private:
    class AKing* m_serverKing;
    class ADrone* m_serverDrone;
    //bool m_isBurnEffectEnabled=true;


public:
    void update(const class UWorld* world);

    void clean();

    //Retrieve the king on the server side. Return nullptr if we're on client side. 
    //Gives an AActor* (i.e this) for the method to check on what side we are...
    FORCEINLINE class AKing* getServerKing(AActor* askingActor) const
    {
        return retrieveIfRoleCorrect(askingActor, m_serverKing);
    }

    //Retrieve the drone on the server side. Return nullptr if we're on client side. 
    //Gives an AActor* (i.e this) for the method to check on what side we are...
    FORCEINLINE class ADrone* getServerDrone(AActor* askingActor) const
    {
        return retrieveIfRoleCorrect(askingActor, m_serverDrone);
    }

//     FORCEINLINE bool IsBurnEffectEnabled() const USE_NOEXCEPT
//     {
//         return m_isBurnEffectEnabled;
//     }
// 
//     FORCEINLINE void setIsBurnEffectEnabled(bool enable)
//     {
//         m_isBurnEffectEnabled = enable;
//     }


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

        typeToUpdate = nullptr;
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

    template<class AskedClass>
    AskedClass* retrieveIfRoleCorrect(AActor* askingActor, AskedClass* toSend) const
    {
        if(askingActor->GetRootComponent()->GetOwnerRole() >= ROLE_Authority)
        {
            return toSend;
        }

        return nullptr;
    }
};