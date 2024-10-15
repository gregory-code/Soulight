// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacterBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDead, bool, bIsDead);

UCLASS()
class ASCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacterBase();

	FOnDead OnDead;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//virtual void OnDead();

	void StartDeath();

public:
	virtual void TakeDamage(float Damage);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float MaxHealth = 100.0f;


};
