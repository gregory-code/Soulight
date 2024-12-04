// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SNPCInteraction.h"

#include "Blueprint/UserWidget.h"

#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

#include "Player/SPlayer.h"

void ASNPCInteraction::BeginPlay()
{
	Super::BeginPlay();

	NPCGUIPopup = CreateWidget<UUserWidget>(GetWorld(), NPCGUIPopupClass);
	
}

void ASNPCInteraction::Interact()
{
	if (!IsValid(Player)) return;

	Player->OnInteract.RemoveDynamic(this, &ASInteractableObject::Interact);

	if (IsValid(NPCGUIPopup))
	{
		NPCGUIPopup->AddToViewport();
	}
	
	if (!IsValid(Player->GetController())) return;

	APlayerController* Controller = Cast<APlayerController>(Player->GetController());
	if (IsValid(Controller))
	{
		Controller->bShowMouseCursor = true;
	}
}

void ASNPCInteraction::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	Super::OnOverlapBegin(overlappedActor, otherActor);
	
}

void ASNPCInteraction::OnOverlapEnd(AActor* overlappedActor, AActor* otherActor)
{
	Super::OnOverlapEnd(overlappedActor, otherActor);

	if (IsValid(NPCGUIPopup))
	{
		NPCGUIPopup->RemoveFromParent();
	}

	if (!IsValid(Player)) return; // Idk why this causing a crash

	if (!IsValid(Player->GetController())) return;

	APlayerController* Controller = Cast<APlayerController>(Player->GetController());
	if (IsValid(Controller))
	{
		Controller->bShowMouseCursor = false;
	}
}
