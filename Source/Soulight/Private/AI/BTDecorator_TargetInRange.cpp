// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_TargetInRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTDecorator_TargetInRange)

UBTDecorator_TargetInRange::UBTDecorator_TargetInRange()
{
	InitNotifyFlags(&UBTDecorator_TargetInRange::TickNode, 
		&UBTDecorator_TargetInRange::OnBecomeRelevant,
		&UBTDecorator_TargetInRange::OnCeaseRelevant,
		&UBTDecorator_TargetInRange::OnNodeActivation,
		&UBTDecorator_TargetInRange::OnNodeDeactivation,
		&UBTDecorator_TargetInRange::OnNodeProcessed
	);

	//FlowAbortMode = EBTFlowAbortMode::Both;
	NodeName = "Target in Range";
}

bool UBTDecorator_TargetInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UE_LOG(LogTemp, Warning, TEXT("Calculate Raw Condition Value	bNotifyProcessed: %d"), bNotifyProcessed)
	return IsTargetInRange(OwnerComp);
}

EBlackboardNotificationResult UBTDecorator_TargetInRange::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	return Super::OnBlackboardKeyValueChange(Blackboard, ChangedKeyID);
	//UE_LOG(LogTemp, Warning, TEXT("Blackboard Key Value Change"))
	//UBehaviorTreeComponent* BehaviorComp = (UBehaviorTreeComponent*)Blackboard.GetBrainComponent();
	//if (BehaviorComp == nullptr)
	//{
	//	return EBlackboardNotificationResult::RemoveObserver;
	//}

	//if (BlackboardKey.GetSelectedKeyID() == ChangedKeyID)
	//{
	//	BehaviorComp->RequestExecution(this); 
	//	//ConditionalFlowAbort(*BehaviorComp, EBTDecoratorAbortRequest::ConditionResultChanged);
	//}

	//return EBlackboardNotificationResult::ContinueObserving;
}

void UBTDecorator_TargetInRange::OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On Node Processed"))
	if (!IsTargetInRange(SearchData.OwnerComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("On Node Processed, not in range"))
		NodeResult = EBTNodeResult::Failed;
		//ConditionalFlowAbort(SearchData.OwnerComp, EBTDecoratorAbortRequest::ConditionResultChanged);
		//SearchData.OwnerComp.RequestBranchDeactivation(*this);
		//SearchData.OwnerComp.StopLogic("No Reason");
		//SearchData.OwnerComp.StopTree();
	}
}

bool UBTDecorator_TargetInRange::IsTargetInRange(UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(GetSelectedBlackboardKey()));
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (AIController && AIController->GetPawn() && TargetActor)
		{
			float Distance = FVector::Distance(AIController->GetPawn()->GetActorLocation(), TargetActor->GetActorLocation());
			return Distance > MinDistance && Distance < MaxDistance;
		}
	}

	return false;
}
