// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define ENABLE_PRINT_ON_SCREEN





#if defined(UE_BUILD_DEBUG) || defined(UE_BUILD_DEVELOPMENT) || defined(UE_BUILD_TEST)
#define WE_RE_ON_DEBUG
#endif //UE_BUILD_DEBUG



#ifdef ENABLE_PRINT_ON_SCREEN /*&& defined(WE_RE_ON_DEBUG)*/

#define PRINT_MESSAGE_ON_SCREEN_UNCHECKED(color, message) GEngine->AddOnScreenDebugMessage(-1, 5.0f, color, message)
#define PRINT_MESSAGE_ON_SCREEN(color, message) if(GEngine) { PRINT_MESSAGE_ON_SCREEN_UNCHECKED(color, message); }

#define PRINT_MESSAGE_TO_TEST_OBJECT_NULLITY(object, color) PRINT_MESSAGE_ON_SCREEN(color, FString(#object) + ((object) ? TEXT(" is not Null") : TEXT(" is Null")))
#define PRINT_MESSAGE_TO_TEST_OBJECT_NULLITY_WITH_PREMESSAGE(message, object, color) PRINT_MESSAGE_TO_TEST_OBJECT_NULLITY(message## #object, color)

#else //!ENABLE_PRINT_ON_SCREEN

#define PRINT_MESSAGE_ON_SCREEN_UNCHECKED(color, message)
#define PRINT_MESSAGE_ON_SCREEN(color, message)

#define PRINT_MESSAGE_TO_TEST_OBJECT_NULLITY(object, color)
#define PRINT_MESSAGE_TO_TEST_OBJECT_NULLITY_WITH_PREMESSAGE(message, object, color)

#endif //ENABLE_PRINT_ON_SCREEN