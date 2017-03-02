// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "LootTable.h"


// Sets default values for this component's properties
ULootTable::ULootTable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULootTable::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULootTable::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void ULootTable::dropItem(const FVector &pos)
{
    TArray<int32> tempProb;
    int indexCopy = 0;
    int32 indexProbMax = m_probs.Num();
    for (indexCopy; indexCopy < indexProbMax; ++indexCopy)
    {
        tempProb.Emplace(m_probs[indexCopy]);
    }
    for (indexCopy; indexCopy < m_objects.Num(); ++indexCopy)
    {
        tempProb.Emplace(m_probs[indexProbMax - 1]);
    } // We can now work on tempProb

    int32 currentMax = 0;
    int32 lastProba = 0;

    UWorld* const world = GetOwner()->GetWorld();
    int32 randomNumber = FMath::RandRange(0, 100);
    for (int i = 0; i < m_objects.Num(); ++i)
    {
        currentMax = lastProba + tempProb[i];
        if (randomNumber < currentMax)
        {
            if (GEngine)
            {
                PRINT_MESSAGE_ON_SCREEN(FColor::Black, "Drop " + FString::FromInt(i));
            }
            world->SpawnActor<AActor>(m_objects[i], pos, FRotator());
            break;
        }
        lastProba = currentMax;
    }
}

