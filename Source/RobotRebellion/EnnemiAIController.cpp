// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "EnnemiAIController.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RobotRebellionCharacter.h"


//ctr ptr method

void AEnnemiAIController::CheckEnnemyNear()
{
    APawn *Pawn = GetPawn();
    FVector MultiSphereStart = Pawn->GetActorLocation();
    FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, 15.0f);
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(Pawn);
    TArray<FHitResult> OutHits;
    bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
                                                                   MultiSphereStart,
                                                                   MultiSphereEnd,
                                                                   700,
                                                                   ObjectTypes,
                                                                   false,
                                                                   ActorsToIgnore,
                                                                   EDrawDebugTrace::ForDuration,
                                                                   OutHits,
                                                                   true);
    //UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
    //BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);


    m_targetToFollow = NULL;

    if(Result == true)
    {
        for(int32 i = 0; i < OutHits.Num(); i++)
        {
            FHitResult Hit = OutHits[i];
            ARobotRebellionCharacter* Character = Cast<ARobotRebellionCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
            if(NULL != Character && Hit.GetActor() == Character)
            {
                //BlackboardComponent->SetValueAsObject("TargetActorToFollow", Character);
                m_targetToFollow = Character;
            }
        }
    }

}
