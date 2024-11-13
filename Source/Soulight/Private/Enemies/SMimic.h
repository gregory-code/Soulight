// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/SEnemy.h"
#include "SMimic.generated.h"

/**
 * 
 */
UCLASS()
class ASMimic : public ASEnemy
{
	GENERATED_BODY()
	
public:
	ASMimic();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void MakeSound_BlueprintEvent();

public:
	UPROPERTY(EditAnywhere, Category = "Interactable")
	class UAnimMontage* InteractMontage;

	UPROPERTY()
	class ASPlayer* Player;

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	class USItemWidgetComponent* ItemWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	class UBoxComponent* GrabBox;

	UFUNCTION()
	virtual void Interact();

	UFUNCTION(BlueprintCallable, Category = "Propeties")
	virtual void OnOverlapBegin(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION(BlueprintCallable, Category = "Propeties")
	virtual void OnOverlapEnd(AActor* overlappedActor, AActor* otherActor);

private:
	ASAIController* AIC;

	virtual void StartDeath(bool IsDead) override;

	float SoundIncrement = 1.0F;

	float LastSound = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Escape")
	float EscapeDelay = 10.0f;

	UFUNCTION()
	void Escape();

};
