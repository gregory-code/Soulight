// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacterBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDead, bool, bIsDead);

class USStatData;

UCLASS()
class ASCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacterBase();

	FOnDead OnDead;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* HitReactionMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* DeathMontage;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void StartDeath();
	
public:
	virtual void TakeDamage(float Damage);

public:
	void AddStats(USStatData* Stats);
	void RemoveStats(USStatData* Stats);

	UFUNCTION()
	float GetHealthStat() const { return Health; }
	UFUNCTION()
	float GetMaxHealthStat() const { return MaxHealth; }
	UFUNCTION()
	float GetStrengthStat() const { return Strength; }
	UFUNCTION()
	float GetDefenseStat() const { return Defense; }
	UFUNCTION()
	float GetAgilityStat() const { return Agility; }
	UFUNCTION()
	float GetSoulStat() const { return Soul; }


	// v rework this v
protected:
	///////////////////////////////
	/*          Stats           */
	/////////////////////////////

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	class UCurveFloat* MoveSpeedCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	class UCurveFloat* AttackSpeedCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Strength = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Defense = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Agility = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Soul = 10.0f;


};