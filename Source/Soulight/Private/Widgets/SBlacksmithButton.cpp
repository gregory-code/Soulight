// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SBlacksmithButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Framework/SEquipmentData.h"

#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "Player/SPlayer.h"

void USBlacksmithButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectBtn) {
		SelectBtn->OnClicked.RemoveDynamic(this, &USBlacksmithButton::OnClicked);
		SelectBtn->OnClicked.AddDynamic(this, &USBlacksmithButton::OnClicked);
	}

	if (ItemName) {
		ItemName->SetText(NameOfItem);
	}
}

void USBlacksmithButton::OnClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController)) return;

	ASPlayer* Player = Cast<ASPlayer>(PlayerController->GetPawn());
	if (!IsValid(Player)) return;

	if (!IsValid(EquipmentObject)) return;
	Player->EquipItem(EquipmentObject);
}
