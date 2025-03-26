// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SDreadstorm_AnimInstance.h"
#include "Animation/Skeleton.h"

#include "Components/SkeletalMeshComponent.h"

#include "Enemies/SDreadstorm.h"

#include "GameFramework/Character.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void USDreadstorm_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

	OwnerDreadstorm = Cast<ASDreadstorm>(OwnerCharacter);

	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &USDreadstorm_AnimInstance::SetupPlayerTarget, 1.f, false, .5f);
}

void USDreadstorm_AnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (OwnerDreadstorm)
	{
		bHeadRaised = OwnerDreadstorm->GetHeadRaised();
		bIsDead = OwnerDreadstorm->GetHeadRaised();

		if (IsAnyMontagePlaying() && PlayerTarget)
		{
			FVector HeadLocation = OwnerDreadstorm->GetMesh()->GetSocketLocation(HeadBoneName);
			FVector PlayerLocation = PlayerTarget->GetActorLocation();

			HeadLookAtRotation = UKismetMathLibrary::FindLookAtRotation(HeadLocation, PlayerLocation);

			HeadLookAtRotation.Yaw += 180.f;  // Adjust the yaw to rotate the head correctly.
		}
	}
}

void USDreadstorm_AnimInstance::SetupPlayerTarget()
{
	PlayerTarget = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

FTransform USDreadstorm_AnimInstance::GetPlayerTransform() const
{
	if (PlayerTarget)
	{
		return PlayerTarget->GetTransform();
	}

	return FTransform();
}

FTransform USDreadstorm_AnimInstance::GetOwnerTransform() const
{
	if (GetOwningActor())
	{
		return GetOwningActor()->GetTransform();
	}

	return FTransform();
}
