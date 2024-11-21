// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SAnimNotifyState_Attack.generated.h"

/**
 * 
 */
UCLASS()
class USAnimNotifyState_Attack : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

	UPROPERTY(EditAnywhere, Category = "Damage")
	float AttackDamage = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float AttackSize = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float Knockback = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float AttackOffset;

private:
	TArray<AActor*> HitActors;

};
