// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SAnimNotify_Attack.generated.h"

/**
 * 
 */
UCLASS()
class USAnimNotify_Attack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float AttackDamage = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float AttackSize = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float Knockback = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float AttackOffset;

};
