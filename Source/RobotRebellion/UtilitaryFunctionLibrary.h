// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilitaryFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class ROBOTREBELLION_API UUtilitaryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
public:
    /*
    Duplicate an object (exact copy) from a default object. The default objecct will be casted to the objInstance type.
    ex :
    while(!UUtilitaryFunctionLibrary::duplicateObjectFromDefault(&objInstance, objDefaultStatic, this, TEXT("youhou")))
    {
        //...
    }
    */
    template<class Object>
    static bool duplicateObjectFromDefault(Object** out, const TSubclassOf<Object>& in, UObject* objectAttachedTo)
    {
        Object* intermediary = DuplicateObject(in.GetDefaultObject(), objectAttachedTo);

        if (intermediary != NULL)
        {
            *out = intermediary;
            return true;
        }

        return false;
    }
	
    /*
    Create an object (exact copy) from a default object. Named version.
    ex : 
    while(!UUtilitaryFunctionLibrary::createObjectFromDefault<objInstanceType>(&objInstance, objDefaultStatic, this, TEXT("youhou")))
    {
        //...
    }
    */
    template<class Casted, class Object>
    static bool createObjectFromDefault(Object** out, const TSubclassOf<Object>& in, UObject* objectAttachedTo, FName name, EObjectFlags RF_flag = RF_Dynamic | RF_ArchetypeObject)
    {
        Object* intermediary = NewObject<Casted>(objectAttachedTo, name, RF_flag, Cast<Casted>(in.GetDefaultObject()));

        if (intermediary != NULL)
        {
            *out = intermediary;
            return true;
        }

        return false;
    }

    /*
    Create an object (exact copy) from a default object. Same as createObjectFromDefault but with no name specified
    */
    template<class Casted, class Object>
    static bool createObjectFromDefault(Object** out, const TSubclassOf<Object>& in, UObject* objectAttachedTo, EObjectFlags RF_flag = RF_Dynamic | RF_ArchetypeObject)
    {
        return createObjectFromDefault<Casted, Object>(out, in, objectAttachedTo, NAME_None, RF_flag);
    }

    /*
    Create an object (exact copy) from a default object. PTR version
    ex :
    while(!UUtilitaryFunctionLibrary::createObjectFromDefault<objInstanceType>(&objInstance, objDefaultStatic, this, TEXT("youhou")))
    {
    //...
    }
    */
    template<class Casted, class Object>
    static bool createObjectFromDefault(Object** out, Object* in, UObject* objectAttachedTo, FName name, EObjectFlags RF_flag = RF_Dynamic | RF_ArchetypeObject)
    {
        Casted* castedIn = Cast<Casted>(in);

        if (castedIn)
        {
            Object* intermediary = NewObject<Casted>(objectAttachedTo, name, RF_flag, castedIn);

            if (intermediary != NULL)
            {
                *out = intermediary;
                return true;
            }
        }
        
        return false;
    }

    /*
    Create an object (exact copy) from a default object. Nameless version
    */
    template<class Casted, class Object>
    static bool createObjectFromDefault(Object** out, Object* in, UObject* objectAttachedTo, EObjectFlags RF_flag = RF_Dynamic | RF_ArchetypeObject)
    {
        return createObjectFromDefault<Casted, Object>(out, in, objectAttachedTo, NAME_None, RF_flag);
    }

    /*
    Randomly applies an effect method from those specified with the parameters equal or above the 3rd parameters
    params :
    - printMessage : bool => true to print what is the effect (in the order passed by parameter)
    - object : the object to those we want to apply methods
    - the remaining parameters : method pointer of the object class type. MUST TAKE NO ARGUMENTS
    */
    template<size_t count, class ObjectTypeToTest, class ... DelegateObj>
    static void randomApplyObjectMethod(bool printMessage, ObjectTypeToTest& object, DelegateObj ... func)
    {
        constexpr int32 totalSize = sizeof...(func);

        if (totalSize == 0)
        {
            return;
        }

        decltype(ObtainFirstElemOnAVariadic(func...)) delegateArray[totalSize] = { func... };

        float coefficient = (static_cast<float>(totalSize) - 0.001f) / (static_cast<float>(RAND_MAX));

        if (printMessage)
        {
            for (size_t iter = 0; iter < count; ++iter)
            {
                int32 randomisator = getRandWithCoeff(coefficient);

                PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Blue, "Executing method : " + FString::FromInt(randomisator));

                (object.*delegateArray[randomisator])();
            }
        }
        else
        {
            for (size_t iter = 0; iter < count; ++iter)
            {
                int32 randomisator = getRandWithCoeff(coefficient);

                (object.*delegateArray[randomisator])();
            }
        }
    }

    template<class FirstElem, class ... OtherElem>
    static constexpr FirstElem ObtainFirstElemOnAVariadic(const FirstElem& first, const OtherElem& ... other)
    {
        return first;
    }


    template<class OwnerType, class PtrMethodType>
    static void bindServerClientMethodPtr(OwnerType owner, PtrMethodType& ptr, PtrMethodType serverMethod, PtrMethodType clientMethod)
    {
        ptr = ((owner->Role < ROLE_Authority) ? clientMethod : serverMethod);
    }

    

    /************************************************************************/
    /* UFUNCTION                                                            */
    /************************************************************************/
    template<class ReturnType> 
    UFUNCTION()
    static FORCEINLINE ReturnType getRandWithCoeff(ReturnType coeff)
    {
        return static_cast<ReturnType>(FMath::Rand()) * coeff;
    }
};
