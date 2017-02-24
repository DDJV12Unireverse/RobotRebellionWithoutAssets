// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define ENABLE_PRINT_ON_SCREEN





#if defined(UE_BUILD_DEBUG) || defined(UE_BUILD_DEVELOPMENT) || defined(UE_BUILD_TEST)
#define WE_RE_ON_DEBUG
#endif //UE_BUILD_DEBUG



#ifdef ENABLE_PRINT_ON_SCREEN /*&& defined(WE_RE_ON_DEBUG)*/

#define PRINT_MESSAGE_ON_SCREEN(color, message) if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.0f, color, message); }

#else //!ENABLE_PRINT_ON_SCREEN

#define PRINT_MESSAGE_ON_SCREEN(color, message)

#endif //ENABLE_PRINT_ON_SCREEN