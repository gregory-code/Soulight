// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/SEnemy.h"
#include "SDreadstorm.generated.h"

/**
 * 
 */
UCLASS()
class ASDreadstorm : public ASEnemy
{
	GENERATED_BODY()

public:
	ASDreadstorm();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TakeDamage(float Damage) override;

	UFUNCTION(BlueprintCallable)
	bool GetHeadRaised() const { return bHeadRaised; }

	UFUNCTION(BlueprintCallable)
	bool GetTransitioning() const { return bTransitioning; }

	UFUNCTION(BlueprintCallable)
	void RaiseHead();

	UFUNCTION(BlueprintCallable)
	void LowerHead();


private:
	UPROPERTY()
	bool bTransitioning = false;

	UPROPERTY()
	bool bHeadRaised = false;

	UPROPERTY(EditDefaultsOnly, Category = "Hurtbox")
	class UBoxComponent* HeadHurtbox;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	class UWidgetComponent* WidgetComponent;

	UPROPERTY()
	class USHealthbar* HealthBar;
};
