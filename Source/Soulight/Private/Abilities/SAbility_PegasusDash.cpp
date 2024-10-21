// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SAbility_PegasusDash.h"

#include "Abilities/SAbilityBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/SPlayer.h"

void ASAbility_PegasusDash::ExecuteAbility()
{
	Super::ExecuteAbility();

	if (GetWorld()->GetTimerManager().IsTimerActive(DashTimer)) return;

	bIsAbilityActive = true;

	GetWorld()->GetTimerManager().SetTimer(DashTimer, this, &ASAbility_PegasusDash::StartDash, 1.0f / 60.0f, true);
}

void ASAbility_PegasusDash::CancelAbility()
{
	Super::CancelAbility();

	EndAbility();

	if (!IsValid(CancelAttackMontage)) return;

	if (!IsValid(OwnerCharacter->GetMesh()) || !IsValid(CancelAttackMontage)) return;

	if (!IsValid(OwnerCharacter->GetMesh()->GetAnimInstance())) return;

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(CancelAttackMontage, 1.0f);
	}

	bIsAbilityActive = false;
}

void ASAbility_PegasusDash::EndAbility()
{
	Super::EndAbility();
	bIsAbilityActive = false;

	EndDash();
}

void ASAbility_PegasusDash::StartDash()
{
	if (!IsValid(OwnerCharacter)) {
		EndDash();
		return;
	}

	FVector OwnerForward = OwnerCharacter->GetActorForwardVector();
	OwnerCharacter->AddMovementInput(OwnerForward, DashSpeed);

	ElapsedTime += GetWorld()->GetDeltaSeconds();
	if (ElapsedTime > DashDuration) EndDash();
}

void ASAbility_PegasusDash::EndDash()
{
	ElapsedTime = 0.0f;

	if (GetWorld()->GetTimerManager().IsTimerActive(DashTimer))
		GetWorld()->GetTimerManager().ClearTimer(DashTimer);
}
