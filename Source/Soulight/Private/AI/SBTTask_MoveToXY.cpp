// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_MoveToXY.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "GameFramework/Character.h"

#include "Navigation/PathFollowingComponent.h"

USBTTask_MoveToXY::USBTTask_MoveToXY()
{
    bNotifyTick = true;
}

EBTNodeResult::Type USBTTask_MoveToXY::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* AIPawn = AIController->GetPawn();
    if (!AIPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp) return EBTNodeResult::Failed;

    // Get the vector location from the specified Blackboard key
    FVector TargetLocation = BlackboardComp->GetValueAsVector(TargetLocationKey.SelectedKeyName);

    // Store the AI’s current Z position to lock it
    InitialZ = AIPawn->GetActorLocation().Z;

    // Only update X and Y, keep the original Z
    FVector AdjustedLocation = FVector(TargetLocation.X, TargetLocation.Y, InitialZ);

    // Move AI to the adjusted location
    AIController->MoveToLocation(AdjustedLocation);

    return EBTNodeResult::InProgress;
}

void USBTTask_MoveToXY::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return;

    APawn* AIPawn = AIController->GetPawn();
    if (!AIPawn) return;

    FVector CurrentLocation = AIPawn->GetActorLocation();

    // Lock the Z position while AI moves in X/Y
    //AIPawn->SetActorLocation(FVector(CurrentLocation.X, CurrentLocation.Y, InitialZ));

    if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
    {
        // AI reached the destination, task is finished
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
