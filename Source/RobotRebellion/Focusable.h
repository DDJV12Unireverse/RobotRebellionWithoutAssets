// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class ROBOTREBELLION_API Focusable
{
public:
	Focusable();
	~Focusable();

    virtual void OnBeginFocus();
    virtual void OnEndFocus();
    virtual void OnPickup(APawn* InstigatorPawn);
    
};
