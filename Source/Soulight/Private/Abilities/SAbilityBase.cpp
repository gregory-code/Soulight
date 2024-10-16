// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SAbilityBase.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Player/Abilities/SAbilityDataBase.h"

USAbilityBase::USAbilityBase()
{

}

void USAbilityBase::CopyAbility(USAbilityBase* AbilityToCopy)
{
	if (IsValid(AbilityToCopy) == false) return;

	USAbilityDataBase* NewAblityData = AbilityToCopy->GetAbilityData();
	if (IsValid(NewAblityData) == false) return;

	AbilityData = NewAblityData;
}

FString USAbilityBase::GetAbilityName() const
{
	if (IsValid(AbilityData) == false)
		return FString("Hello");
	else
		return AbilityData->GetAbilityName();

}

void USAbilityBase::SetOwner(ACharacter* OwningCharacter)
{
	if (!IsValid(OwningCharacter)) return;

	OwnerCharacter = OwningCharacter;
}

void USAbilityBase::ExecuteAbility()
{
	if (!IsValid(OwnerCharacter) || IsValid(AbilityData->GetAnimationMontage()) == false) return;

	PlayMontage(AbilityData->GetAnimationMontage(), AbilityData->GetAnimSpeedMultiplier());
}

void USAbilityBase::PlayMontage(UAnimMontage* MontageToPlay, const float& PlayRate)
{
	if (!IsValid(OwnerCharacter->GetMesh()) || !IsValid(MontageToPlay)) return;

	if (!IsValid(OwnerCharacter->GetMesh()->GetAnimInstance())) return;

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) && AnimInstance->IsAnyMontagePlaying() == false)
	{
		AnimInstance->Montage_Play(MontageToPlay, PlayRate);
	}
}
