// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SItemBase.generated.h"

UCLASS()
class ASItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	class UBoxComponent* GrabBox;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	class USItemWidgetComponent* ItemWidgetComponent;

private:
	UPROPERTY()
	bool bInRange;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	class USAbilityDataBase* AbilityItem;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetAbilityItem(class USAbilityDataBase* ability);

	//UPROPERTY()
	//class EquipmentItem*

	UFUNCTION(BlueprintCallable, Category = "Propeties")
	void Interact(bool bActionable);

	UFUNCTION(BlueprintCallable, Category = "Propeties")
	void OnOverlapBegin(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION(BlueprintCallable, Category = "Propeties")
	void OnOverlapEnd(AActor* overlappedActor, AActor* otherActor);

};
