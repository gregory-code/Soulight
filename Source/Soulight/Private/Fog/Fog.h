// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fog.generated.h"

UCLASS()
class AFog : public AActor
{
	GENERATED_BODY()
	
public:	
	AFog();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Fog Details")
	class UBoxComponent* GrabBox;

	UPROPERTY(VisibleAnywhere, Category = "Fog Details")
	class UStaticMeshComponent* PickupMesh;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void OnOverlapBegin(AActor* overlappedActor, AActor* otherActor);
};
