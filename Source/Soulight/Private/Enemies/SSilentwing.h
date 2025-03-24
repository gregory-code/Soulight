// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/SEnemy.h"
#include "SSilentwing.generated.h"

class USFlyingComponent;
class ACharacter;
/**
 * 
 */
UCLASS()
class ASSilentwing : public ASEnemy
{
	GENERATED_BODY()
	
public:
	ASSilentwing();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float AttackRange = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float FlyRange = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	USFlyingComponent* FlyingComponent;

	ACharacter* Target;
};
