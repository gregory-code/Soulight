// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SCharacterBase.h"

#include "Framework/SStatData.h"
#include "GameFramework/CharacterMovementComponent.h"

ASCharacterBase::ASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

void ASCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASCharacterBase::StartDeath()
{
	OnDead.Broadcast(true);
}

void ASCharacterBase::TakeDamage(float Damage, AActor* DamageInstigator, const float& Knockback)
{
	float TotalDamage = Damage - Defense;

	TotalDamage = TotalDamage < 0 ? 0 : TotalDamage;

	Health = FMath::Clamp(Health - TotalDamage, 0, MaxHealth);

	if (IsValid(HitReactionMontage)) 
	{
		if (IsValid(GetMesh()->GetAnimInstance())) 
		{
			UE_LOG(LogTemp, Warning, TEXT("I am playing hit montage: %s"), *GetOwner()->GetName());

			GetMesh()->GetAnimInstance()->StopAllMontages(1.0f);
			GetMesh()->GetAnimInstance()->Montage_Play(HitReactionMontage);
		}
	}

	if(IsValid(DamageInstigator))
		ApplyKnockback(DamageInstigator->GetActorLocation(), Knockback);

	if (Health <= 0)
	{
		StartDeath();
		UE_LOG(LogTemp, Warning, TEXT("Character Dead"));
	}
}

void ASCharacterBase::ApplyKnockback(const FVector& FromPosition, const float& Knockback)
{
	if (!IsValid(GetCharacterMovement())) return;

	FVector Direction = FromPosition - GetActorLocation();
	Direction = -Direction;

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->Launch(Direction * Knockback);
}

void ASCharacterBase::ApplyStun(const float& Duration, const bool& OverrideCurrentStunDuration)
{
	if (OverrideCurrentStunDuration == true)
	{
		GetWorld()->GetTimerManager().ClearTimer(StunTimerHandle);
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(StunTimerHandle) == true) return;

	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_None;

	GetWorld()->GetTimerManager().SetTimer(StunTimerHandle, this, &ASCharacterBase::EndStun, 1.0f, false, Duration);
}

void ASCharacterBase::EndStun()
{
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
}

void ASCharacterBase::ApplySoulStats(const FSoulMultiplier& Multiplier)
{
	Strength *= Multiplier.GetStrengthMultiplier();
	Defense *= Multiplier.GetDefenseMultiplier();
	Agility *= Multiplier.GetAgilityMultiplier();
	Soul *= Multiplier.GetSoulMultiplier();
}

void ASCharacterBase::AddStats(USStatData* Stats)
{
	MaxHealth += Stats->GetMaxHealthStat();
	Strength += Stats->GetStrengthStat();
	Defense += Stats->GetDefenseStat();
	Agility += Stats->GetAgilityStat();
	Soul += Stats->GetSoulStat();
}

void ASCharacterBase::RemoveStats(USStatData* Stats)
{
	MaxHealth -= Stats->GetMaxHealthStat();
	Strength -= Stats->GetStrengthStat();
	Defense -= Stats->GetDefenseStat();
	Agility -= Stats->GetAgilityStat();
	Soul -= Stats->GetSoulStat();
}
