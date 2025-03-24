// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UIInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IIInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(class ASPlayer* Player);
	virtual void Interact_Implementation(class ASPlayer* Player) = 0;

	UFUNCTION(BlueprintImplementableEvent, Category = "Properties")
	void OnInteract(class ASPlayer* Player);

	UFUNCTION(BlueprintImplementableEvent, Category = "Properties")
	void OnWidgetDisable();

	UFUNCTION(BlueprintImplementableEvent, Category = "Properties")
	void OnWidgetEnable();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EnableInteractionWidget();
	virtual void EnableInteractionWidget_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void DisableInteractionWidget();
	virtual void DisableInteractionWidget_Implementation() = 0;
};
