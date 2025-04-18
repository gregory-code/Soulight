// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

/**
 *
 */
UCLASS()
class ASAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASAIController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

private:
	void SenseSightStimulus(AActor* Target, FAIStimulus Stimulus);
	void SenseHearingStimulus(AActor* Target, FAIStimulus Stimulus);

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnTargetForgotten(AActor* Target);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BehaviorTree;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	bool bUseSightSenseByDefault = true;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	FName AIVisionAttachSocketName = "Head";
	
	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PlayerBBKeyName = "Target";

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName LocationBBKeyName = "TargetLocation";
};