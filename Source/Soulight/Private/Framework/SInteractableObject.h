// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInteractableObject.generated.h"

UCLASS()
class ASInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	class UBoxComponent* GrabBox;

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	class USItemWidgetComponent* ItemWidgetComponent;

	UPROPERTY()
	class ASPlayer* Player;

private:
	UPROPERTY()
	bool bInRange;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class USItemUI* ItemUI;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Interact();

	UFUNCTION(BlueprintCallable, Category = "Propeties")
	virtual void OnOverlapBegin(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION(BlueprintCallable, Category = "Propeties")
	virtual void OnOverlapEnd(AActor* overlappedActor, AActor* otherActor);


};
