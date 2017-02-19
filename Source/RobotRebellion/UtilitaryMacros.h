// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define ENABLE_PRINT_ON_SCREEN





#ifdef ENABLE_PRINT_ON_SCREEN

#define PRINT_MESSAGE_ON_SCREEN(color, message) GEngine->AddOnScreenDebugMessage(-1, 5.0f, color, message);

#else //!ENABLE_PRINT_ON_SCREEN

#define PRINT_MESSAGE_ON_SCREEN(color, message)

#endif //ENABLE_PRINT_ON_SCREEN