// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "BossTrigger.h"


// Sets default values
ABossTrigger::ABossTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_bossTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
    m_bossTriggerBox->bGenerateOverlapEvents = true;
    m_bossTriggerBox->SetRelativeScale3D(FVector(0.5, 3.5, 3.5));

    m_bossTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossTrigger::OnBeginOverlap);
}

// Called when the game starts or when spawned
void ABossTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossTrigger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABossTrigger::OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

