// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


UENUM(BlueprintType)
enum class ELivingTextAnimMode : uint8
{
    TEXT_ANIM_NOT_READY,
    TEXT_ANIM_MOVING,
    TEXT_ANIM_NOT_MOVING,
    TEXT_ANIM_BOING_BOING,
    TEXT_ANIM_BOING_BIGGER_TEXT_ON_CRITICAL
};
