// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SAbilityBase.h"
#include "SAbility_LuckyTail.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class ASAbility_LuckyTail : public ASAbilityBase
{
	GENERATED_BODY()

public:
	virtual void RegisterAbility(ACharacter* OwningCharacter) override;

private:
	UFUNCTION()
	void OnDamaged(float DamageTaken, AActor* DamageInstigator);

	UFUNCTION()
	void OnDamagedEnd();

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* TriggerSound;

	class ASCharacterBase* Player;

};
