// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/SAbilityDataBase.h"

void USAbilityDataBase::LevelUp()
{

}

FString USAbilityDataBase::GetAbilityName()
{
	return AbilityName;
}

float USAbilityDataBase::GetBaseDamage()
{
	return 0.0f;
}

float USAbilityDataBase::GetAnimSpeedMultiplier()
{
	return 0.0f;
}
