// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "EditorGraphVolume.h"
#include "NavigationVolumeGraph.h" 
#include "VolumeIdProvider.h"

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
    m_id = VolumeIdProvider::getInstance().getNextId();
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
    // Only register node if its the server
    if(Role == ROLE_Authority)
    {
        NavigationVolumeGraph::getInstance().addNode(this);
    }
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

float AEditorGraphVolume::isBelow(const FVector& point) const
{
    FVector volumeCenter = GetActorLocation();
    FVector extent = m_box->GetScaledBoxExtent();

    int32 maxY, maxX, minY, minX;
    maxY = volumeCenter.Y + extent.Y;
    maxX = volumeCenter.X + extent.X;
    minY = volumeCenter.Y - extent.Y;
    minX = volumeCenter.X - extent.X;
    if(point.Y < maxY
       && point.X < maxX
       && point.Y > minY
       && point.X > minX)
    {
        return fabs(point.Z - volumeCenter.Z);
    }
    return -1.f;
}