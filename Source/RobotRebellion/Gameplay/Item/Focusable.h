// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class  Focusable
{
public:
//	Focusable();
//	~Focusable();

    virtual void OnBeginFocus()=0;
    virtual void OnEndFocus()=0;
    virtual void OnPickup(APawn* InstigatorPawn)=0;
    
};
