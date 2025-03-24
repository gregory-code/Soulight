// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_TargetInRange.generated.h"

/**
 * 
 */
UCLASS()
class UBTDecorator_TargetInRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_TargetInRange();
private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;
	virtual void OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult);
	UPROPERTY(EditAnywhere, Category = "Range")
	float MinDistance = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Range")
	float MaxDistance = 250.f;

	bool IsTargetInRange(UBehaviorTreeComponent& OwnerComp) const;
};
