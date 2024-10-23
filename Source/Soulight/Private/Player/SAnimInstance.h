// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	bool IsMoving() const { return Speed != 0; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	bool IsNotMoving() const { return Speed == 0; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	FRotator GetLookOffset() const { return LookOffset; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	float GetYawSpeed() const { return YawSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	bool ShouldDoUpperBody() const;

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	bool GetIsAiming() const { return bIsAiming; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	float GetFwdSpeed() const { return FwdSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	float GetRightSpeed() const { return RightSpeed; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* StartMontage;

	// the below functions are the native overrides for each phase
	// Native initialization override point BEGIN PLAY
	virtual void NativeInitializeAnimation() override;

	// Native thread safe update override point. Executed on a worker thread just prior to graph update 
	// for linked anim instances, only called when the hosting node(s) are relevant TICK
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	const ACharacter* OwnerCharacter;
	const UCharacterMovementComponent* OwnerMovementComp;

	float IdleTime = 0;

	float Speed;
	FRotator LookOffset;

	FRotator PrevRot;
	float YawSpeed;

	float FwdSpeed;
	float RightSpeed;

	bool bIsAiming;

	void AimingTagChanged(bool state);
};
