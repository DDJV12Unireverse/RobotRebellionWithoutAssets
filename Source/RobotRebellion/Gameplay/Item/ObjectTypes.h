#pragma once

//Object Type enum

UENUM(BlueprintType)
enum class EObjectType : uint8
{
    NONE,
    BOMB,
    HEALTH_POTION,
    MANA_POTION,

    TYPE_COUNT
};