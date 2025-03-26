// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SDreadstorm_AnimInstance.generated.h"

class ACharacter;
class ASDreadstorm;
/**
 * 
 */
UCLASS()
class USDreadstorm_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	bool GetHeadRaised() const { return bHeadRaised; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	bool GetIsDead() const { return bIsDead; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	bool IsAnyMontagePlayerThreadSafe() const{ return IsAnyMontagePlaying(); }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	FRotator GetHeadLookAtRotation() const { return HeadLookAtRotation; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	FTransform GetPlayerTransform() const;

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	FTransform GetOwnerTransform() const;


private:
	// the below functions are the native overrides for each phase
	// Native initialization override point BEGIN PLAY
	virtual void NativeInitializeAnimation() override;

	// Native thread safe update override point. Executed on a worker thread just prior to graph update 
	// for linked anim instances, only called when the hosting node(s) are relevant TICK
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void SetupPlayerTarget();

	FName HeadBoneName = "jnt_head";

	const ACharacter* OwnerCharacter;
	const ASDreadstorm* OwnerDreadstorm;

	const APawn* PlayerTarget;

	FRotator HeadLookAtRotation;

	bool bHeadRaised;
	bool bIsDead;
};
