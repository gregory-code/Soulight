// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SAbility_LuckyTail.h"
#include "Abilities/SAbilityBase.h"

#include "Framework/SCharacterBase.h"

#include "Kismet/GameplayStatics.h"

void ASAbility_LuckyTail::RegisterAbility(ACharacter* OwningCharacter)
{
	Super::RegisterAbility(OwningCharacter);

	if (!IsValid(OwnerCharacter)) return;

	Player = Cast<ASCharacterBase>(OwnerCharacter);
	if (IsValid(Player))
	{
		Player->OnDamageTaken.AddDynamic(this, &ASAbility_LuckyTail::OnDamaged);
		Player->OnDamageTakenEnd.AddDynamic(this, &ASAbility_LuckyTail::OnDamagedEnd);
	}
}

void ASAbility_LuckyTail::OnDamaged(float DamageTaken, AActor* DamageInstigator)
{
	if (!IsValid(Player)) return;

	if(IsValid(DamageInstigator))
		Player->ApplyKnockback(DamageInstigator->GetActorLocation(), DamageTaken);

	
	const float DodgeChance = 0.3f;
	if (FMath::FRand() <= DodgeChance)
	{
		if (IsValid(TriggerSound))
			UGameplayStatics::PlaySound2D(this, TriggerSound);

		UE_LOG(LogTemp, Warning, TEXT("LUCKY TAIL DODGED THE DAMAGE!"));
		Player->SetDodge(true);
	}
}

void ASAbility_LuckyTail::OnDamagedEnd()
{
	if (!IsValid(Player)) return;

	Player->SetDodge(false);
}
