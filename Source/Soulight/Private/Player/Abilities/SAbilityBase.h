// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SAbilityBase.generated.h"

UCLASS()
class ASAbilityBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASAbilityBase();

	UFUNCTION()
	void LevelUp();

	UFUNCTION()
	FString GetAbilityName();

	UFUNCTION()
	float GetBaseDamage();

	UFUNCTION()
	float GetAnimSpeedMultiplier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	int level;

	UPROPERTY(EditDefaultsOnly, Category = "Values")
	FString AbilityName;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	class UAnimMontage* AbilityAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Values")
	TArray<float> BaseDamage = { 0, 0, 0 };

	UPROPERTY(EditDefaultsOnly, Category = "Values")
	TArray<float> AnimSpeedMultiplier = { 0,0,0 };
};
