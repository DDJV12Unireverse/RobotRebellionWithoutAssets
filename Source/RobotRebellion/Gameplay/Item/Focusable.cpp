// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Focusable.h"

//Focusable::Focusable()
//{
//    
//}
//
//Focusable::~Focusable()
//{
//}
void Focusable::OnBeginFocus()
{}
void Focusable::OnEndFocus()
{}
void Focusable::OnPickup(APawn* InstigatorPawn)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Cyan,"FOCUSABLE");
}