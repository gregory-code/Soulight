// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/SAbilityBase.h"

// Sets default values
ASAbilityBase::ASAbilityBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASAbilityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASAbilityBase::LevelUp()
{

}

FString ASAbilityBase::GetAbilityName()
{
	return AbilityName;
}

float ASAbilityBase::GetBaseDamage()
{
	return BaseDamage[level];
}

float ASAbilityBase::GetAnimSpeedMultiplier()
{
	return AnimSpeedMultiplier[level];
}
