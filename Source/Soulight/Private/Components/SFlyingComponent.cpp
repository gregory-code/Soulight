// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SFlyingComponent.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Player/SPlayer.h"

USFlyingComponent::USFlyingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USFlyingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>(GetOwner());
}

void USFlyingComponent::StartFly()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(FlightTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(FlightTimerHandle);
		FlightTimerHandle.Invalidate();
	}

	if (!IsValid(OwningCharacter)) return;

	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.f;

	bIsFlying = true;
	bLerpingHeight = true;

	GetWorld()->GetTimerManager().SetTimer(FlightTimerHandle, this, &USFlyingComponent::LerpHeightUp, 1/60.f, true);
}

void USFlyingComponent::StopFly(const FVector& LandingLocation)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(FlightTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(FlightTimerHandle);
		FlightTimerHandle.Invalidate();
	}

	if (!IsValid(OwningCharacter)) return;

	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.f;

	GroundTargetLocation = LandingLocation;

	bIsFlying = false;
	bLerpingHeight = true;

	GetWorld()->GetTimerManager().SetTimer(FlightTimerHandle, this, &USFlyingComponent::SwoopDown, 1 / 60.f, true);
}
void USFlyingComponent::LerpHeightUp()
{
	if (!IsValid(OwningCharacter) && !IsValid(OwningCharacter->GetCapsuleComponent())) return;

	float NewHalfHeight = FMath::FInterpTo
	(
		OwningCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(),
		500.f,
		GetWorld()->GetDeltaSeconds(),
		2.0f
	);

	OwningCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);

	if (FMath::Abs(OwningCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - 500.f) <= 1.f)
	{
		bLerpingHeight = false;

		GetWorld()->GetTimerManager().ClearTimer(FlightTimerHandle);
	}
}

// This needs to calculate distance from player and adjust swoop speed based on distance
void USFlyingComponent::SwoopDown()
{
	if (!IsValid(OwningCharacter) && !IsValid(OwningCharacter->GetCapsuleComponent())) return;

	float NewHalfHeight = FMath::FInterpTo
	(
		OwningCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(),
		60.f,
		GetWorld()->GetDeltaSeconds(),
		10.0f
	);

	FVector LaunchDirection = GroundTargetLocation - OwningCharacter->GetActorLocation();
	LaunchDirection.Normalize();

	OwningCharacter->GetCharacterMovement()->Launch(LaunchDirection * 1000.f);

	OwningCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);

	// max distance / current distance, clamp to 1 * max speed

	float AttackRadius = 75.0f;
	FVector AttackCenter = GetOwner()->GetActorLocation();

	// 
	if (!bReportedHit) 
	{
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->OverlapMultiByObjectType(
			OverlapResults,
			AttackCenter,
			FQuat::Identity,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
			FCollisionShape::MakeSphere(AttackRadius),
			QueryParams
		);

		if (bHit)
		{
			for (const FOverlapResult& Result : OverlapResults)
			{
				ASPlayer* Player = Cast<ASPlayer>(Result.GetActor());
				if (IsValid(Player))
				{
					Player->CharacterTakeDamage(35.0f, GetOwner(), 25.0f);
					bReportedHit = true;
				}
			}
		}

		//DrawDebugSphere(GetWorld(), AttackCenter, AttackRadius, 32, FColor::Red, false, 0.2f);
	}

	if (FMath::Abs(OwningCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - 60.f) <= 1.f)
	{
		bLerpingHeight = false;

		bReportedHit = false;

		OnLanded.Broadcast();

		GetWorld()->GetTimerManager().ClearTimer(FlightTimerHandle);
	}
}
