// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SDreadstorm.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Widget.h"

#include "Components/SkeletalMeshComponent.h"

#include "Widgets/SHealthbar.h"

ASDreadstorm::ASDreadstorm()
{
	HeadHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Head Hurtbox"));
	HeadHurtbox->SetupAttachment(GetRootComponent());

	LeftHandHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Hand Hitbox"));
	LeftHandHitbox->SetupAttachment(GetRootComponent());

	RightHandHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Hand Hitbox"));
	RightHandHitbox->SetupAttachment(GetRootComponent());
}

void ASDreadstorm::BeginPlay()
{
	Super::BeginPlay();

	HealthBar = CreateWidget<USHealthbar>(GetWorld(), HealthBarWidget);
	HealthBar->AddToViewport();

	if (IsValid(GetMesh()))
	{
		if (GetMesh()->DoesSocketExist(LeftHandSocketName))
		{
			LeftHandHitbox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandSocketName);
		}

		if (GetMesh()->DoesSocketExist(LeftHandSocketName))
		{
			RightHandHitbox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandSocketName);
		}
	}

	if (!IsValid(HealthBar)) return;

	// Force a layout prepass to ensure the widget's components are ready
	HealthBar->SynchronizeProperties();
	HealthBar->ForceLayoutPrepass();

	HealthBar->UpdateHealthbar(Health, MaxHealth);
}

void ASDreadstorm::CharacterTakeDamage(float Damage, AActor* DamageInstigator, const float& Knockback)
{
	Super::CharacterTakeDamage(Damage, DamageInstigator, 0);

	if (!IsValid(HealthBar)) return;

	CheckPhaseState();

	if (Health <= 0)
	{
		HealthBar->RemoveFromViewport();
	}

	HealthBar->UpdateHealthbar(Health, MaxHealth);
}

void ASDreadstorm::CheckPhaseState()
{
	if (bPhaseTwo == false && Health > 300 && Health <= 600) // Phase 2
	{
		HeadHurtbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetVisibility(false);
		bTransitioning = true;

		bPhaseTwo = true;

		GetWorld()->GetTimerManager().SetTimer(TransitionTimerHandle, this, &ASDreadstorm::DragonTransitioningState, 1.0f, false, TransitionDuration);
	}
	else if (bPhaseThree == false && Health <= 300) // Phase 3 
	{
		HeadHurtbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetVisibility(false);
		bTransitioning = true;

		bPhaseTwo = false;
		bPhaseThree = true;

		// Double the duration idk, if this is too easy for player make more fireballs
		GetWorld()->GetTimerManager().SetTimer(TransitionTimerHandle, this, &ASDreadstorm::DragonTransitioningState, 1.0f, false, TransitionDuration * 2);
	}
	// else I am in phase 1

}

void ASDreadstorm::DragonTransitioningState()
{
	HeadHurtbox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetVisibility(true);
	bTransitioning = false;

	if (bPhaseTwo)
	{
		FRotator TargetRotation = FRotator(0.0f, 90.0f, 0.0f);
		FQuat RotationQuat = FQuat(TargetRotation);

		SetActorLocationAndRotation(LeftStagePosition, RotationQuat, false, nullptr, ETeleportType::None);
	}
	else if (bPhaseThree)
	{
		FRotator TargetRotation = FRotator(0.0f, -90.0f, 0.0f);
		FQuat RotationQuat = FQuat(TargetRotation);

		SetActorLocationAndRotation(RightStagePosition, RotationQuat, false, nullptr, ETeleportType::None);
	}
}

bool ASDreadstorm::GetIsDead()
{
	return Health <= 0;
}

void ASDreadstorm::RaiseHead()
{
	HeadHurtbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bHeadRaised = true;
}

void ASDreadstorm::LowerHead()
{
	HeadHurtbox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	bHeadRaised = false;
}


