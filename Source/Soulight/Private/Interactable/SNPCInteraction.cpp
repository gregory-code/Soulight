// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SNPCInteraction.h"

#include "Blueprint/UserWidget.h"

void ASNPCInteraction::BeginPlay()
{
	Super::BeginPlay();

	NPCGUIPopup = CreateWidget<UUserWidget>(GetWorld(), NPCGUIPopupClass);
	
}

void ASNPCInteraction::Interact()
{

}

void ASNPCInteraction::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	Super::OnOverlapBegin(overlappedActor, otherActor);

	if (IsValid(NPCGUIPopup))
	{
		NPCGUIPopup->AddToViewport();
	}
}

void ASNPCInteraction::OnOverlapEnd(AActor* overlappedActor, AActor* otherActor)
{
	Super::OnOverlapEnd(overlappedActor, otherActor);


	if (IsValid(NPCGUIPopup))
	{
		NPCGUIPopup->RemoveFromParent();
	}
}
