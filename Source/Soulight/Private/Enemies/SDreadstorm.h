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
	virtual void TakeDamage(float Damage, AActor* DamageInstigator, const float& Knockback) override;

	UFUNCTION(BlueprintCallable)
	bool GetHeadRaised() const { return bHeadRaised; }

	UFUNCTION(BlueprintCallable)
	bool GetTransitioning() const { return bTransitioning; }

	UFUNCTION(BlueprintCallable)
	void RaiseHead();

	UFUNCTION(BlueprintCallable)
	void LowerHead();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hurtbox")
	class UBoxComponent* HeadHurtbox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hitbox")
	class UBoxComponent* LeftHandHitbox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hitbox")
	class UBoxComponent* RightHandHitbox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bTransitioning = false;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Hitbox")
	FName LeftHandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Hitbox")
	FName RightHandSocketName;

	UPROPERTY()
	bool bHeadRaised = false;

	UPROPERTY()
	class USHealthbar* HealthBar;
};
