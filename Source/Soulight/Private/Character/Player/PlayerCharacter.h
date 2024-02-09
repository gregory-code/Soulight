// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public: 
	APlayerCharacter();
	
private:
	UPROPERTY(visibleAnywhere, Category = "Camera")
	USpringArmComponent* cameraBoom;

	UPROPERTY(visibleAnywhere, Category = "Camera")
	UCameraComponent* viewCamera;

	virtual void PawnClientRestart() override;

	/********************************************/
	/*                Input                     */
	/********************************************/

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* inputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* moveInputAction;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& InputValue);

	FVector GetMoveFwdDir() const;
	FVector GetMoveRightDir() const;
};
