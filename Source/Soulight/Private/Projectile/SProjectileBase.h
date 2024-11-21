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

protected:
	virtual void BeginPlay() override;

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
