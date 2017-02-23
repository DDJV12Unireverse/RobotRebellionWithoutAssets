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
	
};
