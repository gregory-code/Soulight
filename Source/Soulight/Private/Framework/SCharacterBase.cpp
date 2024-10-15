// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SCharacterBase.h"

// Sets default values
ASCharacterBase::ASCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;
}

// Called every frame
void ASCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASCharacterBase::StartDeath()
{
	OnDead.Broadcast(true);
}

void ASCharacterBase::TakeDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);

	if (Health <= 0) 
	{
		StartDeath();
	}
}

