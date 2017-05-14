// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#define ENABLE_PRINT_ON_SCREEN
//#define ENABLE_SPELL_DISPLAY_SHERECAST
//#define ENABLE_DRAW_DEBUG_LINE
//#define ENABLE_DRONE_DEBUG_DISPLAY





#if defined(UE_BUILD_DEBUG) || defined(UE_BUILD_TEST)
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

#ifdef ENABLE_SPELL_DISPLAY_SHERECAST

#define SPHERECAST_DISPLAY_ONE_FRAME EDrawDebugTrace::ForOneFrame
#define SPHERECAST_DISPLAY_DURATION EDrawDebugTrace::ForDuration
#define SPHERECAST_DISPLAY_PERSISTENT EDrawDebugTrace::Persistent
#define SPHERECAST_DISPLAY_NONE EDrawDebugTrace::None

#else 

#define SPHERECAST_DISPLAY_ONE_FRAME EDrawDebugTrace::None
#define SPHERECAST_DISPLAY_DURATION EDrawDebugTrace::None
#define SPHERECAST_DISPLAY_PERSISTENT EDrawDebugTrace::None
#define SPHERECAST_DISPLAY_NONE EDrawDebugTrace::None

#endif // ENABLE_SPELL_DISPLAY_SHERECAST

#ifdef ENABLE_DRAW_DEBUG_LINE

#define DRAW_DEBUG_LINE(world, startLocation, endLocation, color) DrawDebugLine(world, startLocation, endLocation, color, false, 5.f);

#else

#define DRAW_DEBUG_LINE(world, startLocation, endLocation, color) 

#endif // ENABLE_DRAW_DEBUG_LINE



/************************************************************************/
/* NET                                                                  */
/************************************************************************/
#define GENERATE_DEFAULT_VALIDATION_METHOD(className, funcName, ...) \
bool className::funcName##_Validate(__VA_ARGS__) \
{ \
    return true; \
} \

#define GENERATE_IMPLEMENTATION_METHOD_AND_DEFAULT_VALIDATION_METHOD(className, funcName, ...) \
GENERATE_DEFAULT_VALIDATION_METHOD(className, funcName, __VA_ARGS__) \
void className::funcName##_Implementation(__VA_ARGS__)



/************************************************************************/
/* CLIENT                                                               */
/************************************************************************/

#define GENERATE_PROTOTYPE_SERVER_CLIENT_METHODS_BASED_VALIDATION_SERVER(MethodPtrName, className, funcName, argument) protected: \
    void (className::* MethodPtrName)(argument); \
    void funcName##ClientImp(argument); \
    void funcName##ServerImp(argument);


#define GENERATE_DECLARATION_SERVER_CLIENT_METHODS_BASED_VALIDATION_SERVER_FROM_METHOD_PTR(methodPtrName, className, uPropFuncName, serverFuncName, argumentType, argumentName) \
GENERATE_DEFAULT_VALIDATION_METHOD(className, serverFuncName, argumentType) \
void className::serverFuncName##_Implementation(argumentType argumentName) \
{ \
    (this->*methodPtrName)(argumentName); \
} \
void className::uPropFuncName(argumentType argumentName) \
{ \
    (this->*methodPtrName)(argumentName); \
} \


#define GENERATE_DECLARATION_SERVER_CLIENT_METHODS_BASED_VALIDATION_SERVER_FROM_METHOD_PTR_WITH_CLIENT_IMPL_GEN(methodPtrName, className, uPropFuncName, serverFuncName, argumentType, argumentName) \
GENERATE_DECLARATION_SERVER_CLIENT_METHODS_BASED_VALIDATION_SERVER_FROM_METHOD_PTR(methodPtrName, className, uPropFuncName, serverFuncName, argumentType, argumentName) \
void className::uPropFuncName##ClientImp(argumentType argumentName) \
{ \
    serverFuncName(argumentName); \
}
