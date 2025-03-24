// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/SEnemy.h"
#include "SDreadstorm.generated.h"

class UAnimMontage;
/**
 *
 */
UCLASS()
class ASDreadstorm : public ASEnemy
{
	GENERATED_BODY()

public:
	ASDreadstorm();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void CharacterTakeDamage(float Damage, AActor* DamageInstigator, const float& Knockback) override;

	UFUNCTION(BlueprintCallable)
		bool GetHeadRaised() const { return bHeadRaised; }

	UFUNCTION(BlueprintCallable)
		bool GetTransitioning() const { return bTransitioning; }

	UFUNCTION()
		bool GetIsDead();

	UFUNCTION(BlueprintCallable)
		void RaiseHead();

	UFUNCTION(BlueprintCallable)
		void LowerHead();

protected:
	UFUNCTION()
	void LateSetup();

	bool InRange(const FVector& CurrentLocation, const FVector& PlayerLocation) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hurtbox")
		class UBoxComponent* HeadHurtbox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hitbox")
		class UBoxComponent* LeftHandHitbox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hitbox")
		class UBoxComponent* RightHandHitbox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bTransitioning = false;

private:
	class ACharacter* PlayerRefer;

	FVector NewPlayerLocation;

	void SelectAttack();
	UPROPERTY(EditDefaultsOnly, Category = "Attack Animations")
	TArray<UAnimMontage*> AttackAnimations;

	void CheckPhaseState();

	UFUNCTION()
	void DragonTransitioningState();
	FTimerHandle TransitionTimerHandle;
	const float TransitionDuration = 5.0f;

	// probably not the best approach but this is easier for me
	bool bPhaseTwo = false;
	bool bPhaseThree = false;

	UPROPERTY(EditDefaultsOnly, Category = "Location")
		FVector LeftStagePosition;

	UPROPERTY(EditDefaultsOnly, Category = "Location")
		FVector RightStagePosition;

	UPROPERTY(EditDefaultsOnly, Category = "Location")
		FVector CenterStagePosition;

	UPROPERTY(EditDefaultsOnly, Category = "Hitbox")
		FName LeftHandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Hitbox")
		FName RightHandSocketName;

	UPROPERTY()
		bool bHeadRaised = false;

	UPROPERTY()
		class USHealthbar* HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UUserWidget> HealthBarWidget;

};
