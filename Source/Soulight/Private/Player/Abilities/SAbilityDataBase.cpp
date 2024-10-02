// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/SAbilityDataBase.h"

bool USAbilityDataBase::LevelUp()
{
	level++;

	if (level >= 3)
	{
		level = 0;
		return false;
	}

	return true;
}

EType USAbilityDataBase::GetType()
{
	return AbilityType;
}

FString USAbilityDataBase::GetAbilityName()
{
	return AbilityName;
}

FString USAbilityDataBase::GetAbilityDescription()
{
	return AbilityDescription;
}

UTexture* USAbilityDataBase::GetAbilityIcon()
{
	return AbilityIcon;
}

float USAbilityDataBase::GetBaseDamage()
{
	return BaseDamage[level];
}

float USAbilityDataBase::GetAnimSpeedMultiplier()
{
	return AnimSpeedMultiplier[level];
}
