// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFlyingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLanded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAirborne);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USFlyingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USFlyingComponent();

	FOnLanded OnLanded;
	FOnAirborne OnAirborne;

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void StartFly();

	UFUNCTION()
	void StopFly(const FVector& LandingLocation);

	UFUNCTION()
	void LerpHeightUp();

	UFUNCTION()
	void SwoopDown();

	UFUNCTION(BlueprintCallable)
	bool GetIsFlying() const { return bIsFlying; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Flying")
	bool bIsFlying = true;

	UPROPERTY()
	bool bReportedHit = false;

	float TargetCapsuleHalfHeight = 60.f;
	bool bLerpingHeight = false;

	FVector GroundTargetLocation;

	UPROPERTY()
	class ACharacter* OwningCharacter;

	FTimerHandle FlightTimerHandle;
};
