// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SGameplayUI.h"

#include "Components/Image.h"
#include "Engine/Texture2D.h"

void USGameplayUI::NativeConstruct()
{
	Super::NativeConstruct();


}

void USGameplayUI::SetSkillImage(UTexture* targetImage)
{
	if (!IsValid(targetImage)) return;

	if (!IsValid(SkillImage)) return;

	SkillImage->GetDynamicMaterial()->SetTextureParameterValue(IconMaterialParameterName, targetImage);
}

void USGameplayUI::SetSpellImage(UTexture* targetImage)
{
	if (!IsValid(targetImage)) return;

	if (!IsValid(SpellImage)) return;

	SpellImage->GetDynamicMaterial()->SetTextureParameterValue(IconMaterialParameterName, targetImage);
}
