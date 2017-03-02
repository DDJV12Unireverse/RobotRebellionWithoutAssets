// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "PickupActor.h"


// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = MeshComp;

    MeshComp->SetSimulatePhysics(true);
    bReplicates = true;
    bReplicateMovement = true;
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickupActor::OnBeginFocus()
{
    // Highlight PostProcess
    MeshComp->SetRenderCustomDepth(true);
}

void APickupActor::OnEndFocus()
{
    // Stop Highlight PostProcess
    MeshComp->SetRenderCustomDepth(false);
}

void APickupActor::OnPickup(APawn * InstigatorPawn)
{
    //Nothing. To be derived.
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("PickedUp"));
    if (Role==ROLE_Authority)
    Destroy();
   /*    if (Role < ROLE_Authority)
       {
           serverOnPickup(InstigatorPawn);
       }*/
    //UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
}
//void APickupActor::serverOnPickup_Implementation(APawn * InstigatorPawn)
//{
//    OnPickup(InstigatorPawn);
//}
//bool APickupActor::serverOnPickup_Validate(APawn * InstigatorPawn)
//{
//    return true;
//}