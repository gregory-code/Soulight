// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SLineageEntry.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"

void USLineageEntry::InitLineageEntry(const FString& CharacterName, const FString& Personality,
	UTexture2D* SkillIcon, UTexture2D* SpellIcon, UTexture2D* PassiveIcon, UTextureRenderTarget2D* CharacterRenderTarget,
	UTexture2D* InheritedAbility)
{
	FText NameTextFormatted = FText::FromString("Name: " + CharacterName);
	CharacterNameText->SetText(NameTextFormatted);

	FText PersonalityFormatted = FText::FromString("Personality: " + Personality);
	CharacterPersonalityText->SetText(PersonalityFormatted);

	if (CharacterPreviewImage && IsValid(CharacterRenderTarget))
	{
		UMaterialInstanceDynamic* DynamicMaterial = CharacterPreviewImage->GetDynamicMaterial();
		if (DynamicMaterial)
		{
			DynamicMaterial->SetTextureParameterValue(TEXT("RenderTarget2D"), CharacterRenderTarget);
		}
	}

	if (SkillIcon)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(SkillIcon);
		CharacterSkillIcon->SetBrush(Brush);
	}

	if (SpellIcon)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(SpellIcon);
		CharacterSpellIcon->SetBrush(Brush);
	}

	if (PassiveIcon)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(PassiveIcon);
		CharacterPassiveIcon->SetBrush(Brush);
	}

	if (InheritedAbility)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(InheritedAbility);
		CharacterInheritedAbilityIcon->SetBrush(Brush);
	}

}
