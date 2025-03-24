// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Framework/SGameInstance.h"
#include "SDialogueHandlerComponent.generated.h"

class ASPlayer;
class UDataTable;

// Player Damage Events
// 
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageDealt, ASCharacterBase*, Target);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageTaken, float, DamageTaken, AActor*, Instigator);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTakenEnd);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKill);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USDialogueHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USDialogueHandlerComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FString GetResponseFromDataTable(FPersonalities PersonalityStruct, const FString& Personality) const;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	ASPlayer* Player;
	
private: // Combat Events
	UFUNCTION()
	void OnDamageTaken(float DamageTaken, AActor* Instigator);

	UFUNCTION()
	void OnKill();

	UFUNCTION()
	void OnCombatStarted();
	UFUNCTION()
	void OnCombatEnded();


	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	UDataTable* OnKillDialogueTable;

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	UDataTable* OnCombatStartedDialogueTable;

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	UDataTable* OnCombatEndedDialogueTable;

	UFUNCTION()
	void ResetKillDialogueDelay();

	float KillDelayTimeout = 1.5f;
	FTimerHandle OnKillDialogueDelay;
	bool bKillDelay = false;

private: // Silentwing Noise Events
	UFUNCTION()
	void OnNoiseReported(float TotalNoiseLevel);
	bool bHasNoiseBeenReported = false;

	UPROPERTY()
	float LastNoiseLevelReported = 0.f;

	UFUNCTION()
	void OnNoiseReset();

public:
	UFUNCTION()
	void EvaluateInteraction(AActor* InteractedObject);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	TMap<UClass*, UDataTable*> InteractObjectDialagueMap;

	void SayDialogue(UDataTable* DialogueTable);

};
