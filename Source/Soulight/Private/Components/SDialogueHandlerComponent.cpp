
#include "Components/SDialogueHandlerComponent.h"

#include "AI/SSilentwingNoiseManager.h"

#include "Player/SPlayer.h"

#include "Framework/SGameInstance.h"

#include "Interactable/SChest.h"

USDialogueHandlerComponent::USDialogueHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USDialogueHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ASPlayer>(GetOwner());

	if (!IsValid(Player)) return;

	USSilentwingNoiseManager::Get(GetOwner()).OnNoiseReported.AddDynamic(this, &USDialogueHandlerComponent::OnNoiseReported);
	USSilentwingNoiseManager::Get(GetOwner()).OnNoiseReset.AddDynamic(this, &USDialogueHandlerComponent::OnNoiseReset);

	Player->OnDamageTaken.AddDynamic(this, &USDialogueHandlerComponent::OnDamageTaken);
	Player->OnKill.AddDynamic(this, &USDialogueHandlerComponent::OnKill);

	Player->OnInteract.AddDynamic(this, &USDialogueHandlerComponent::EvaluateInteraction);

	Player->OnCombatStarted.AddDynamic(this, &USDialogueHandlerComponent::OnCombatStarted);
	Player->OnCombatEnded.AddDynamic(this, &USDialogueHandlerComponent::OnCombatEnded);
}

void USDialogueHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USDialogueHandlerComponent::OnDamageTaken(float DamageTaken, AActor* Instigator)
{
	float CurrentHealth = (Player->GetHealthStat() / Player->GetMaxHealthStat());

	if (CurrentHealth <= 0.5f)
	{
		Player->Speak("My health is getting low");
	}
}

void USDialogueHandlerComponent::OnKill()
{
	if (!OnKillDialogueTable || bKillDelay) 
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(OnKillDialogueDelay, this, &USDialogueHandlerComponent::ResetKillDialogueDelay, 1, false, KillDelayTimeout);

	SayDialogue(OnKillDialogueTable);

	bKillDelay = true;
}

void USDialogueHandlerComponent::OnCombatStarted()
{
	if (!OnCombatStartedDialogueTable)
	{
		return;
	}

	SayDialogue(OnCombatStartedDialogueTable);
}

void USDialogueHandlerComponent::OnCombatEnded()
{
	if (!OnCombatEndedDialogueTable)
	{
		return;
	}

	SayDialogue(OnCombatEndedDialogueTable);
}

void USDialogueHandlerComponent::ResetKillDialogueDelay()
{
	bKillDelay = false;
}

void USDialogueHandlerComponent::OnNoiseReported(float TotalNoiseLevel)
{
	LastNoiseLevelReported = TotalNoiseLevel;

	bool SeenSilentWing = USSilentwingNoiseManager::Get(GetOwner()).IsFirstSpawn();
	FString Text = SeenSilentWing ? "a" : "another";

	if (TotalNoiseLevel >= 100.f)
	{
		Player->Speak("Damn, looks like I alerted " + Text + " silentwing!");

	}
	else if (TotalNoiseLevel >= 50.f && !bHasNoiseBeenReported)
	{
		Player->Speak("I need to stay quiet, " + Text + " silentwing is nearby.");

		bHasNoiseBeenReported = true;
	}
	else if (TotalNoiseLevel < 50.f)
	{
		bHasNoiseBeenReported = false;
	}
}

void USDialogueHandlerComponent::OnNoiseReset()
{
	if (LastNoiseLevelReported > 50)
	{
		bool SeenSilentWing = USSilentwingNoiseManager::Get(GetOwner()).IsFirstSpawn();
		FString Text = SeenSilentWing ? "a" : "another";

		Player->Speak("Phew... I think I avoided alerting " + Text + " silentwing.");
	}
}

void USDialogueHandlerComponent::EvaluateInteraction(AActor* InteractedObject)
{
	if (!InteractedObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interacted Object Is NULL!"));
		return;
	}

	if (InteractObjectDialagueMap.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractObject Dialague Map Is Empty!"));
		return;
	}

	UClass* ObjectClass = InteractedObject->GetClass();
	UClass* BaseClass = ObjectClass->GetSuperClass(); // Get the base C++ class

	if (!BaseClass)
	{
		BaseClass = ObjectClass; // Fallback in case there is no superclass.
	}

	if (InteractObjectDialagueMap.Contains(BaseClass))
	{
		if (UDataTable* InteractionResponseTable = InteractObjectDialagueMap[BaseClass])
		{
			SayDialogue(InteractionResponseTable);
		}
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Map Does Not Contain Class! AActor: %s"),
			*InteractedObject->GetName(), *BaseClass->GetName());
	}
}

void USDialogueHandlerComponent::SayDialogue(UDataTable* DialogueTable)
{
	if (!Player || !DialogueTable)
	{
		return;
	}

	USGameInstance* SGameInstance = Cast<USGameInstance>(GetWorld()->GetGameInstance());
	if (!SGameInstance)
	{
		return;
	}

	int32 ResponseRow = FMath::RandRange(1, DialogueTable->GetRowMap().Num());

	FString RowName = FString::Printf(TEXT("Response_%d"), ResponseRow);
	FPersonalities* Personality = DialogueTable->FindRow<FPersonalities>(FName(*RowName), FString(""));
	if (!Personality)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Saying Dialogue!"));
	Player->Speak(GetResponseFromDataTable(*Personality, SGameInstance->GetCurrentPersonality().ToString()));
}

FString USDialogueHandlerComponent::GetResponseFromDataTable(FPersonalities PersonalityStruct, const FString& Personality) const
{
	if (Personality == "Adventurous")
	{
		return PersonalityStruct.Adventurous;
	}

	if (Personality == "Ruthless")
	{
		return PersonalityStruct.Ruthless;
	}

	if (Personality == "Paranoid")
	{
		return PersonalityStruct.Paranoid;
	}

	if (Personality == "Stoic")
	{
		return PersonalityStruct.Stoic;
	}

	if (Personality == "Calm")
	{
		return PersonalityStruct.Calm;
	}

	if (Personality == "Desperate")
	{
		return PersonalityStruct.Desperate;
	}

	return FString();
}
