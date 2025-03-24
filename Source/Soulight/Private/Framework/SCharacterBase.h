// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacterBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDead, bool, bIsDead, AActor*, DeadActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageDealt, ASCharacterBase*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageTaken, float, DamageTaken, AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTakenEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKill);

class USStatData;

USTRUCT(BlueprintType)
struct FSoulMultiplier 
{
	GENERATED_BODY()

public:
	float GetStrengthMultiplier() const { return Strength; }
	float GetDefenseMultiplier() const { return Defense; }
	float GetAgilityMultiplier() const { return Agility; }
	float GetSoulMultiplier() const { return Soul; }

private:
	UPROPERTY(EditAnywhere, Category = "Stat Multiplier")
	float Strength = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Stat Multiplier")
	float Defense = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Stat Multiplier")
	float Agility = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Stat Multiplier")
	float Soul = 1.0f;
};

// Probably not the best use case rn but am currently using this for the lamp light stat bonus stuff
UENUM(BlueprintType)
enum class ESoulStatType : uint8 
{
	Strength	UMETA(DisplayName = "Strength"),
	Defense		UMETA(DisplayName = "Defense"),
	Agility		UMETA(DisplayName = "Agility"),
	Soul		UMETA(DisplayName = "Soul"),
	None		UMETA(DisplayName = "None")
};

UCLASS()
class ASCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacterBase();

	FOnDead OnDead;
	FOnDamageTaken OnDamageTaken;
	FOnDamageTakenEnd OnDamageTakenEnd;
	FOnKill OnKill;
	FOnDamageDealt OnDamageDealt;

	bool GetIsDamageable() const { return bIsDamageable; }
	
	UFUNCTION(BlueprintCallable)
		bool IsStunned() const { return bStunned; }

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

	UFUNCTION(BlueprintImplementableEvent)
	void StartDeath_BlueprintEvent();

private:
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* HitSound;

public:
	UFUNCTION(BlueprintCallable)
	virtual void CharacterTakeDamage(float Damage, AActor* DamageInstigator, const float& Knockback);

	UFUNCTION()
	void ApplyKnockback(const FVector& FromPosition, const float& Knockback);

	UFUNCTION()
	void ApplyStun(const float& Duration, const bool& OverrideCurrentStunDuration);
	UFUNCTION()
	void EndStun();

	FTimerHandle StunTimerHandle;

public:
	void ApplySoulStats(const FSoulMultiplier& Multiplier);
	void RemoveSoulStats(const FSoulMultiplier& Multiplier);

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

public:
	void SetDodge(bool state);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	bool bCanKnockback = true;

	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	bool bIsDamageable = true;

	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	bool bStunned = false;

	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	bool bDodge = false;

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