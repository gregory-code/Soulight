// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SAbilityBase.h"
#include "SAbility_PegasusDash.generated.h"

/**
 * 
 */
UCLASS()
class ASAbility_PegasusDash : public ASAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteAbility() override;
	virtual void CancelAbility() override;
	virtual void EndAbility() override;

private:
	UFUNCTION()
	void StartDash();
	void EndDash();

	UPROPERTY(EditDefaultsOnly, Category = "Pegasus Dash")
	class UAnimMontage* CancelAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Pegasus Dash")
	float DashSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Pegasus Dash")
	float DashDuration = 2.0f;

	float ElapsedTime = 0.0f;
	FTimerHandle DashTimer;
};

