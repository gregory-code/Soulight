// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFog.generated.h"

UCLASS()
class ASFog : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASFog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	bool bCursedFog;

	UPROPERTY(VisibleAnywhere, Category = "Fog")
	bool bIsCleaning;


	UFUNCTION()
	void SetIsCleaning(bool state);

	UFUNCTION()
	void TryClean();

private:

	UPROPERTY(VisibleAnywhere, Category = "Particles")
	class UStaticMeshComponent* VolumeFogComponent;

	UPROPERTY(VisibleAnywhere, Category = "Particles")
	class UStaticMesh* VolumeFogMesh;

	void Reappear();

	FTimerHandle ReappearTimerHandle;
};
