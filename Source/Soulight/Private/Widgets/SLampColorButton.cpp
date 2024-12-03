// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SLampColorButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "Player/SPlayer.h"

void USLampColorButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectBtn) {
		SelectBtn->OnClicked.RemoveDynamic(this, &USLampColorButton::OnClicked);
		SelectBtn->OnClicked.AddDynamic(this, &USLampColorButton::OnClicked);
	}

	if (PassiveDescription) {
		PassiveDescription->SetText(PassiveText);
	}
}

void USLampColorButton::OnClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController)) return;

	ASPlayer* Player = Cast<ASPlayer>(PlayerController->GetPawn());
	if (!IsValid(Player)) return;

	Player->SetSoulBuff(SoulStatType);
}
