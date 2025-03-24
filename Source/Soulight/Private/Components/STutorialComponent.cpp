// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STutorialComponent.h"

#include "Blueprint/UserWidget.h"

#include "Framework/SGameInstance.h"

#include "GameFramework/PlayerController.h"

#include "Player/SPlayer.h"

#include "Kismet/GameplayStatics.h"

#include "Interactable/SNPCInteraction.h"

#include "Widgets/STutorialPopup.h"

USTutorialComponent::USTutorialComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USTutorialComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    Player = Cast<ASPlayer>(GetOwner());

	SGameInstance = Cast<USGameInstance>(GetWorld()->GetGameInstance());
	if (SGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance is being set in Tutorial Component"));
	}
}

void USTutorialComponent::ShowTutorialPopup(TSubclassOf<USTutorialPopup> TutorialPopupClass)
{
	if (!IsValid(TutorialPopupClass) || !IsValid(OwningController))
	{
		return;
	}

    USTutorialPopup* HintWidget = CreateWidget<USTutorialPopup>(OwningController, TutorialPopupClass);
	if (HintWidget)
	{
		HintWidget->AddToViewport();

        if (Player)
        {
            HintWidget->Init(Player);
            Player->SetPlayerInputEnabled(false);
        }

        OwningController->bShowMouseCursor = true;
		OwningController->SetPause(true); // Pause game
	}
}

void USTutorialComponent::EvaluateObject(AActor* ObjectToEvaluate)
{
    if (!SGameInstance || !ObjectToEvaluate)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed To Evaluate Object"));
        return;
    }

    if (!SGameInstance->HasSeenBothNPCs())
    {
        SGameInstance->SeenNPC(ObjectToEvaluate);
    }

    UClass* ObjectClass = ObjectToEvaluate->GetClass();
    UClass* BaseClass = ObjectClass->GetSuperClass(); // Get the base C++ class

    if (!BaseClass)
    {
        BaseClass = ObjectClass; // Fallback in case there is no superclass.
    }

    FString ObjectName = ObjectToEvaluate->GetName();
    FString ClassName = BaseClass->GetName();

    // Check if we've already seen the tutorial for this class
    if (SGameInstance->HasSeenTutorial(BaseClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("Tutorial Already Seen for: %s (Type: %s)"), *ObjectName, *ClassName);
        return;
    }

    if (TSubclassOf<USTutorialPopup>* FoundWidget = TutorialWidgets.Find(BaseClass))
    {
        ShowTutorialPopup(*FoundWidget);

        SGameInstance->MarkTutorialSeen(BaseClass);
    }
}
