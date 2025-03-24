// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SNPCInteraction.h"

#include "Blueprint/UserWidget.h"

#include "Components/SphereComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"

#include "Player/SPlayer.h"

ASNPCInteraction::ASNPCInteraction()
{
	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interact Sphere"));
	InteractSphere->SetupAttachment(GetRootComponent());

	InteractSphere->SetSphereRadius(128.f);
	InteractSphere->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	InteractSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
}

void ASNPCInteraction::BeginPlay()
{
	Super::BeginPlay();

	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &ASNPCInteraction::PlayerLeftInteractRange);

	NPCGUIPopup = CreateWidget<UUserWidget>(GetWorld(), NPCGUIPopupClass);
}

void ASNPCInteraction::InteractWithNPC(ASPlayer* Player)
{
	if (bIsInteracting)
	{
		RemovePopupFromViewport(Player);
	}
	else
	{
		AddPopupToViewport(Player);
	}
}

void ASNPCInteraction::AddPopupToViewport(class ASPlayer* Player)
{
	if (IsValid(NPCGUIPopup))
	{
		NPCGUIPopup->AddToViewport();
	}

	if (!IsValid(Player)) return;

	if (!IsValid(Player->GetController())) return;

	APlayerController* Controller = Cast<APlayerController>(Player->GetController());
	if (IsValid(Controller))
	{
		Controller->bShowMouseCursor = true;
	}

	bIsInteracting = true;
}

void ASNPCInteraction::RemovePopupFromViewport(class ASPlayer* Player)
{
	if (IsValid(NPCGUIPopup))
	{
		NPCGUIPopup->RemoveFromParent();
	}

	if (!IsValid(Player)) return; 

	if (!IsValid(Player->GetController())) return;

	APlayerController* Controller = Cast<APlayerController>(Player->GetController());
	if (IsValid(Controller))
	{
		Controller->bShowMouseCursor = false;
	}

	bIsInteracting = false;
}

void ASNPCInteraction::PlayerLeftInteractRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsInteracting == false) return;

	if (ASPlayer* Player = Cast<ASPlayer>(OtherActor))
	{
		RemovePopupFromViewport(Player);
	}
}
