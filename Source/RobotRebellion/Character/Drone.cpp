// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Drone.h"
#include "WidgetComponent.h"


ADrone::ADrone() : ANonPlayableCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    this->setImmortal(true);

    this->GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Drone");
    this->GetCharacterMovement()->GravityScale = 0.f;

    //m_utScores = CreateDefaultSubobject<UWidgetComponent>(TEXT("ScoresWidget"));
}

void ADrone::BeginPlay()
{
    //m_textBillboardInstance->
}
void ADrone::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}

void ADrone::displayScore(float scores[5])
{
    if (this->getBillboardComponent())
    {
        this->displayAnimatedText(FString::Printf(TEXT("Follow :%f \n Reload :%f \n Attack :%f \n Drop :%f \n Wait :%f"),
            scores[0], scores[1], scores[2], scores[3], scores[4]), FColor::Blue, ELivingTextAnimMode::TEXT_ANIM_NOT_MOVING);
        //this->displayAnimatedText(FString::Printf(TEXT("Reload :%f"), scores[1]), FColor::Blue, ELivingTextAnimMode::TEXT_ANIM_MOVING);
        //this->displayAnimatedText(FString::Printf(TEXT("Attack :%f"), scores[2]), FColor::Blue, ELivingTextAnimMode::TEXT_ANIM_NOT_MOVING);
        //this->displayAnimatedText(FString::Printf(TEXT("Drop :%f"), scores[3]), FColor::Blue, ELivingTextAnimMode::TEXT_ANIM_NOT_MOVING);
        //this->displayAnimatedText(FString::Printf(TEXT("Wait :%f"), scores[4]), FColor::Blue, ELivingTextAnimMode::TEXT_ANIM_NOT_MOVING);

    }
}
