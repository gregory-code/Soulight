// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

UCLASS()
class ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectileBase();

public:
	void EnableOrbit(AActor* OrbitTarget);
	void DisableOrbit();

	void LaunchAtTarget(AActor* Target);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool bShouldOrbit = false;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float OrbitSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float OrbitRadius = 3.0f;

	UPROPERTY()
	float OrbitAngle = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	AActor* OrbitCenter;

private:
	UPROPERTY()
	AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float LerpSpeed = 1000.0f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Velocity = 2500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Damage = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Knockback = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	class USphereComponent* SphereCollider;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	class UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ASCharacterBase> TargetClass;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
