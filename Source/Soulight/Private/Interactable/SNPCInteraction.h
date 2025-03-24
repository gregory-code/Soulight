// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SInteractableObject.h"
#include "SNPCInteraction.generated.h"

/**
 * 
 */
UCLASS()
class ASNPCInteraction : public ASInteractableObject
{
	GENERATED_BODY()

public:
	ASNPCInteraction();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void InteractWithNPC(class ASPlayer* Player);

	UFUNCTION(BlueprintCallable)
	void AddPopupToViewport(class ASPlayer* Player);

	UFUNCTION(BlueprintCallable)
	void RemovePopupFromViewport(class ASPlayer* Player);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	class USphereComponent* InteractSphere;

	UFUNCTION()
	void PlayerLeftInteractRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> NPCGUIPopupClass;

	UPROPERTY()
	class UUserWidget* NPCGUIPopup;

	bool bIsInteracting = false;

};
