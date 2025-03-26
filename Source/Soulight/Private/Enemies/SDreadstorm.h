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

	UFUNCTION(BlueprintCallable)
	void SetRightHandHitboxEnabled(bool ActiveState);

	UFUNCTION(BlueprintCallable)
	void SetLeftHandHitboxEnabled(bool ActiveState);

	UFUNCTION()
	bool GetIsDead();

	UFUNCTION(BlueprintCallable)
	void RaiseHead();

	UFUNCTION(BlueprintCallable)
	void LowerHead();

protected:
	void PlayMontage(UAnimMontage* MontageToPlay);
	void StopAllMontages();

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

	UFUNCTION()
	void RightHandOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void RightHandOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void LeftHandOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void LeftHandOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	void DealDamage(class ASCharacterBase* TargetCharacter);

	bool bHitOnce = false;

private:
	// probably not the best approach but this is easier for me
	bool bPhaseTwo = false;
	bool bPhaseThree = false;

	int32 MovesTillNextHeadMovement = 3;

	void CheckPhaseState();

	UFUNCTION()
	void DragonTransitioningState();

	FTimerHandle TransitionTimerHandle;
	const float TransitionDuration = 5.0f;

	void SelectAttack();

	UFUNCTION()
	void WaitForNextAttack();

	UPROPERTY(EditDefaultsOnly, Category = "Location")
	FVector LeftStagePosition;

	UPROPERTY(EditDefaultsOnly, Category = "Location")
	FVector RightStagePosition;

	UPROPERTY(EditDefaultsOnly, Category = "Location")
	FVector CenterStagePosition;

	UPROPERTY()
	bool bHeadRaised = false;

	FTimerHandle AttackResetTimer;
	bool bCanAttack = true;

private:
	class ACharacter* PlayerTarget;

	FVector NewPlayerLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* StartMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EmergeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DigDownMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TArray<UAnimMontage*> RangedAttackAnimations;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ClawAttackAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Hitbox")
	FName LeftHandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Hitbox")
	FName RightHandSocketName;

	UPROPERTY()
	class USHealthbar* HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HealthBarWidget;

};
