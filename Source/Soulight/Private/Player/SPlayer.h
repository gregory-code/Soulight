// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SCharacterBase.h"
#include "SPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ASPlayer : public ASCharacterBase
{
	GENERATED_BODY()

public:
	ASPlayer();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Input")
	class ASPlayerController* PlayerController;

	UPROPERTY(visibleAnywhere, Category = "Camera")
	class USceneComponent* FullHealthView;

	UPROPERTY(visibleAnywhere, Category = "Camera")
	class USceneComponent* EmptyHealthView;

	UPROPERTY(visibleAnywhere, Category = "View")
	class UCameraComponent* MainCamera;

	virtual void PawnClientRestart() override;

	///////////////////////////////
	/*         Inputs           */
	/////////////////////////////

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* AimInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* InteractInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* AttackInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* DodgeInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SkillInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SpellInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* HUDInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SettingsInputAction;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& InputValue);

	UFUNCTION()
	void Aim(const FInputActionValue& InputValue);

	UFUNCTION()
	void Interact();

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void Dodge();

	UFUNCTION()
	void Skill();

	UFUNCTION()
	void Spell();

	UFUNCTION()
	void HUD();

	UFUNCTION()
	void Settings();

	///////////////////////////////
	/*       Movement           */
	/////////////////////////////

	FVector GetMoveFwdDir() const;
	FVector GetMoveRightDir() const;
	FVector previousDir;
};
