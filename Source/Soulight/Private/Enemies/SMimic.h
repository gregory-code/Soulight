// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/SEnemy.h"
#include "SMimic.generated.h"

/**
 * 
 */
UCLASS()
class ASMimic : public ASEnemy
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void MakeSound_BlueprintEvent();

private:
	virtual void StartDeath(bool IsDead) override;

	float SoundIncrement = 1.0F;

	float LastSound = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Escape")
	float EscapeDelay = 10.0f;

	UFUNCTION()
	void Escape();

};
