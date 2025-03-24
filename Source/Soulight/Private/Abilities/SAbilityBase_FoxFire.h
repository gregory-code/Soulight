// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/SAbilityBase.h"
#include "SAbilityBase_FoxFire.generated.h"

class ASProjectileBase;
/**
 * 
 */
UCLASS()
class ASAbilityBase_FoxFire : public ASAbilityBase
{
	GENERATED_BODY()

public:
	ASAbilityBase_FoxFire();

	virtual void Tick(float DeltaTime) override;

	virtual void RegisterAbility(ACharacter* OwningCharacter) override;
	virtual void UnRegisterAbility() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Detection")
	class USphereComponent* DetectionSphere;

	UFUNCTION()
		void OnEnemyDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

private:
	UFUNCTION()
	void OnKill();

	void CreateFoxFireball();

	void DestroyFoxFireballs();

private:
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* TriggerSound;

	UPROPERTY(EditDefaultsOnly, Category = "Fox Fire")
	TSubclassOf<class ASProjectileBase> FoxFireballClass;

	TArray<ASProjectileBase*> InstancedFoxFireballs;

	class ASCharacterBase* Player;

};
