// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SAbilityBase.h"
#include "SAbility_SpinSlash.generated.h"

/**
 * 
 */
UCLASS()
class ASAbility_SpinSlash : public ASAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteAbility() override;

	// Overriding these so they don't do base functionality
	virtual void CancelAbility() override;
	virtual void EndAbility() override;

private:
	UFUNCTION()
	void StartSpinSlash();

	UPROPERTY(EditDefaultsOnly, Category = "Pegasus Dash")
	float DashSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Pegasus Dash")
	float DashDuration = 2.0f;

	float ElapsedTime = 0.0f;
	FTimerHandle DashTimer;
};
