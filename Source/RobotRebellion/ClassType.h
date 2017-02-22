// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


//CLASS FLAG
UENUM(BlueprintType)
enum class EClassType : uint8
{
    NONE        UMETA(DisplayName = "None"),
    SOLDIER     UMETA(DisplayName = "Soldier"),
    ASSASSIN    UMETA(DisplayName = "Assassin"),
    HEALER      UMETA(DisplayName = "Healer"),
    WIZARD      UMETA(DisplayName = "Wizard"),

    TYPE_COUNT
};