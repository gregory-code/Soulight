// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

#include "SBTTask_MoveToXY.generated.h"

/**
 * 
 */
UCLASS()
class USBTTask_MoveToXY : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
    USBTTask_MoveToXY();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    float InitialZ;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
    FBlackboardKeySelector TargetLocationKey;

};
