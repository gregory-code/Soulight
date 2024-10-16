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
	class USAbilityBase* AbilityItem;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FColor AbilityUpgradeColor;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FColor AbilityNewColor;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FColor AbilityReplacesColor;

	UPROPERTY()
	class ASPlayer* Player;

	UPROPERTY(VisibleAnywhere)
	class USItemUI* ItemUI;

	UFUNCTION()
	void SetAbilityItem(class USAbilityBase* ability, FString upgrade, FColor abilityColor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY()
	//class EquipmentItem*

	UFUNCTION(BlueprintCallable, Category = "Propeties")
	void Interact(bool bActionable);

	UFUNCTION(BlueprintCallable, Category = "Propeties")
	void OnOverlapBegin(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION(BlueprintCallable, Category = "Propeties")
	void OnOverlapEnd(AActor* overlappedActor, AActor* otherActor);

};
