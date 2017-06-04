// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "DamageZone.h"
#include "Character/RobotRebellionCharacter.h"


// Sets default values
ADamageZone::ADamageZone() : AActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADamageZone::BeginPlay()
{
    Super::BeginPlay();

    m_secondBetweenTick = 1.f / m_tickRate;
    PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Blue, "spawn molotov zone - tick" + FString::SanitizeFloat(m_secondBetweenTick));
    m_burnedActors = 0;
}

// Called every frame
void ADamageZone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(m_deltaSinceLastTick + DeltaTime > m_secondBetweenTick)// time to deal damage
    {
        
        m_deltaSinceLastTick = 0;// Reset delta

        // proceed sphere cast
        FVector MultiSphereStart = GetActorLocation();
        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(this);
        TArray<FHitResult> hitActors;
        bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
                                                                       MultiSphereStart,
                                                                       MultiSphereStart,
                                                                       m_radius,
                                                                       m_objectTypes,
                                                                       false,
                                                                       ActorsToIgnore,
                                                                       SPHERECAST_DISPLAY_DURATION,
                                                                       hitActors,
                                                                       true);
        //         GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Tick after : " 
        //                                          + FString::SanitizeFloat(m_deltaSinceLastTick)
        //                                          + " hit : " + FString::FromInt(hitActors.Num()));
                // hitActors now countains all actor that should get damage
        for(FHitResult& currentHit : hitActors)
        {
            ARobotRebellionCharacter* temp = Cast<ARobotRebellionCharacter>(currentHit.GetActor());
            if(temp)
            {
                // this is brut damage 
                // TODO - use Damage class and more complexe damage calcul
                if(m_isMolotov && m_burnedActors<=5)
                {
                    ++m_burnedActors;
                    FVector tempLocation = currentHit.Location;
                    tempLocation.Z = 0;
                    temp->spawnFireEffect(tempLocation);
                }
                temp->inflictDamage(m_damagePerTick);
            }
        }
    }
    else
    {
        m_deltaSinceLastTick += DeltaTime;
    }
}

