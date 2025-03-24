// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SAnimNotifyState_InRange.generated.h"

/**
 * 
 */
UCLASS()
class USAnimNotifyState_InRange : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);

	bool IsTargetInRange(AActor* OwnerActor) const;

protected:
	UPROPERTY();
	ACharacter* Target;

	UPROPERTY();
	AActor* OwningActor;

	UPROPERTY(EditAnywhere, Category = "Range")
	float MinDistance = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Range")
	float MaxDistance = 250.0f;

};
