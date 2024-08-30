// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFogCleaner.generated.h"

UCLASS()
class ASFogCleaner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASFogCleaner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void SetColliderRadius(const float newRadius);

private:

	UPROPERTY(VisibleAnywhere, Category = "Cleaner")
	class USphereComponent* SphereCollider;

	UFUNCTION(BlueprintCallable, Category = "Cleaner")
	void OnOverlapBegin(AActor* overlappedActor, AActor* otherActor);
};
