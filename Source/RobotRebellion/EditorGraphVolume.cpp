// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "EditorGraphVolume.h"

// Sets default values
AEditorGraphVolume::AEditorGraphVolume() 
    : AActor(), m_isVisibleInGame{true}
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Init box comp to have editor visual feedback
    m_box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    m_box->SetHiddenInGame(!m_isVisibleInGame); // enable to be shown in game
    m_box->SetCollisionEnabled(ECollisionEnabled::NoCollision); // disable collision
}

// Called when the game starts or when spawned
void AEditorGraphVolume::BeginPlay()
{
    Super::BeginPlay();
    //m_id = Identificateur::get().getNextId();
    registerNode();
}

// Called every frame
void AEditorGraphVolume::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Graph registration
void AEditorGraphVolume::registerNode()
{
    //Graph::getInstance()->addNode(this);
}

bool AEditorGraphVolume::contains(const FVector& point)const
{
    FVector volumeCenter = GetActorLocation();
    FVector extent = m_box->GetScaledBoxExtent();

    int32 maxZ, maxY, maxX, minZ, minY, minX;
    maxZ = volumeCenter.Z + extent.Z;
    maxY = volumeCenter.Y + extent.Y;
    maxX = volumeCenter.X + extent.X;
    minZ = volumeCenter.Z - extent.Z;
    minY = volumeCenter.Y - extent.Y;
    minX = volumeCenter.X - extent.X;
    if(   point.Z < maxZ
       && point.Y < maxY
       && point.X < maxX
       && point.Z > minZ
       && point.Y > minY
       && point.X > minX)
    {
        return true;
    }
    return false;
}