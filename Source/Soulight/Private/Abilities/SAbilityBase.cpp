// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SAbilityBase.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Player/Abilities/SAbilityDataBase.h"

ASAbilityBase::ASAbilityBase()
{

}

FString ASAbilityBase::GetAbilityName() const
{
	if (IsValid(AbilityData) == false)
		return FString("Hello");
	else
		return AbilityData->GetAbilityName();

}

void ASAbilityBase::SetAbilityOwner(ACharacter* OwningCharacter)
{
	if (!IsValid(OwningCharacter)) return;

	OwnerCharacter = OwningCharacter;
}

void ASAbilityBase::ExecuteAbility()
{
	if (!IsValid(OwnerCharacter) || !IsValid(AbilityData->GetAnimationMontage())) return;

	PlayMontage(AbilityData->GetAnimationMontage(), AbilityData->GetAnimSpeedMultiplier());
}

void ASAbilityBase::CancelAbility()
{

}

void ASAbilityBase::EndAbility()
{
	if (!IsValid(OwnerCharacter) || !IsValid(AbilityData->GetAnimationMontage())) return;

	StopMontage(AbilityData->GetAnimationMontage());
}

void ASAbilityBase::PlayMontage(UAnimMontage* MontageToPlay, const float& PlayRate)
{
	if (!IsValid(OwnerCharacter->GetMesh()) || !IsValid(MontageToPlay)) return;

	if (!IsValid(OwnerCharacter->GetMesh()->GetAnimInstance())) return;

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) && !AnimInstance->Montage_IsPlaying(MontageToPlay))
	{
		AnimInstance->Montage_Play(MontageToPlay, PlayRate);
	}
}

void ASAbilityBase::StopMontage(UAnimMontage* MontageToStop)
{
	if (!IsValid(OwnerCharacter->GetMesh()) || !IsValid(MontageToStop)) return;

	if (!IsValid(OwnerCharacter->GetMesh()->GetAnimInstance())) return;

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Stop(0.0f, MontageToStop);
	}
}