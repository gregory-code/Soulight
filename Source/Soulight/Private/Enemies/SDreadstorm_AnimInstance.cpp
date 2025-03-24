// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SDreadstorm_AnimInstance.h"

#include "Enemies/SDreadstorm.h"

#include "GameFramework/Character.h"

void USDreadstorm_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

	OwnerDreadstorm = Cast<ASDreadstorm>(OwnerCharacter);
}

void USDreadstorm_AnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (OwnerDreadstorm)
	{
		bHeadRaised = OwnerDreadstorm->GetHeadRaised();
		bIsDead = OwnerDreadstorm->GetHeadRaised();
	}
}
