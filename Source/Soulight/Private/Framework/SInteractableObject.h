// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/IInteractable.h"
#include "SInteractableObject.generated.h"

UCLASS()
class ASInteractableObject : public AActor, public IIInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	class UBoxComponent* RootCollider;

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	class USItemWidgetComponent* ItemWidgetComponent;
	 
	UPROPERTY(EditAnywhere, Category = "Interactable")
	class UAnimMontage* InteractMontage;

	UPROPERTY()
	bool bInRange;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class USItemUI* ItemUI;

public:	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Interact_Implementation(class ASPlayer* Player) override;

	// Inherited via IIInteractable
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void EnableInteractionWidget_Implementation() override;

	// Inherited via IIInteractable
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void DisableInteractionWidget_Implementation() override;
};

