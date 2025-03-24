// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SGameInstance.h"

#include "Abilities/SAbilityBase.h"
#include "AI/SSilentwingNoiseManager.h"

#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/StaticMesh.h"

#include "Framework/SFog.h"

#include "GameFramework/Actor.h"
#include "GameFramework/PlayerInput.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"

#include "Interactable/SNPCInteraction.h"

#include "Lineage/SLineageCharacterRenderPreview.h"

#include "Widgets/SMinimap.h"

FString USGameInstance::GetResponse(const FName& Personality, const FString& InteractionName) const
{
    if (!MyDataTable) return FString("Data Table Is Null!");

	FDialogueData* Row = MyDataTable->FindRow<FDialogueData>(Personality, FString(""));

    if (!Row) return FString("Row Is Null!");

    FString Response = "!";
    FName InteractionPropertyName = FName(*InteractionName);

    UStruct* RowStruct = FDialogueData::StaticStruct();
    for (TFieldIterator<FProperty> PropIt(RowStruct); PropIt; ++PropIt)
    {
        FProperty* Property = *PropIt;

        if (Property->GetFName() == InteractionPropertyName)
        {
            FStrProperty* StrProp = CastField<FStrProperty>(Property);
            if (StrProp)
            {
                Response = StrProp->GetPropertyValue_InContainer(Row);
                break;
            }
        }
    }

	return Response;
}

FDialogueData* USGameInstance::GetResponsesForPersonality() const
{
	if (!MyDataTable) return nullptr;
	return	MyDataTable->FindRow<FDialogueData>(GetCurrentPersonality(), FString(""));
}

void USGameInstance::ClearEquippedItems()
{
	SavedSkillName = "";
	SavedSpellName = "";
	SavedPassiveName = "";

	EquippedItems.EquippedWeapon = nullptr;
	EquippedItems.EquippedChest = nullptr;
	EquippedItems.EquippedHead = nullptr;
	EquippedItems.EquippedBoot = nullptr;
}

USSilentwingNoiseManager* USGameInstance::GetNoiseManager()
{
	if (!SilentwingNoizeManager && SilentwingNoiseManagerClass)
	{
		SilentwingNoizeManager = NewObject<USSilentwingNoiseManager>(this, SilentwingNoiseManagerClass);
	}

	return SilentwingNoizeManager;
}

void USGameInstance::SetMinimap(USMinimap* Minimap)
{
	MinimapInstance = Minimap;
}

void USGameInstance::InitPreviewCharacter()
{
	if (!IsValid(LineageCharacterPreviewClass)) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	LineageCharacterRenderPreview = GetWorld()->SpawnActor<ASLineageCharacterRenderPreview>(LineageCharacterPreviewClass, FVector(0, 0, -1000.0f), FRotator::ZeroRotator, SpawnParams);
}

void USGameInstance::UpdateRenderPreviewEquipment(const FLineageCharacterData& Entry, const EEquipmentType& EquipmentType, UStaticMesh* StaticMesh)
{
	if (!IsValid(LineageCharacterRenderPreview)) return;

	LineageCharacterRenderPreview->UpdatePreviewEquipment(EquipmentType, StaticMesh);

	LineageCharacterRenderPreview->CaptureSnapshot(Entry.RenderTarget);
}

void USGameInstance::CaptureChararcterPreview()
{
	if (!IsValid(LineageCharacterRenderPreview)) return;

	for (FLineageCharacterData data : PreviousLineageEntries) 
	{
		if(IsValid(data.EquippedItems.EquippedBoot))
			LineageCharacterRenderPreview->UpdatePreviewEquipment(
				data.EquippedItems.EquippedBoot->EquipmentType,
				data.EquippedItems.EquippedBoot->EquipmentMesh
			);

		if (IsValid(data.EquippedItems.EquippedChest))
			LineageCharacterRenderPreview->UpdatePreviewEquipment(
				data.EquippedItems.EquippedChest->EquipmentType,
				data.EquippedItems.EquippedChest->EquipmentMesh
			);	

		if (IsValid(data.EquippedItems.EquippedHead))
			LineageCharacterRenderPreview->UpdatePreviewEquipment(
				data.EquippedItems.EquippedHead->EquipmentType,
				data.EquippedItems.EquippedHead->EquipmentMesh
			);

		if (IsValid(data.EquippedItems.EquippedWeapon))
			LineageCharacterRenderPreview->UpdatePreviewEquipment(
				data.EquippedItems.EquippedWeapon->EquipmentType,
				data.EquippedItems.EquippedWeapon->EquipmentMesh
			);

		LineageCharacterRenderPreview->CaptureSnapshot(data.RenderTarget);

		LineageCharacterRenderPreview->ResetEquipmentDisplay();
		UE_LOG(LogTemp, Warning, TEXT("Capturing"))
	}
}

void USGameInstance::SaveEquipment()
{
	PreviousLineageEntries.Last().EquippedItems = EquippedItems;
}


void USGameInstance::StartLineage()
{
	PreviousLineageEntries.Add(
		FLineageCharacterData(Personalities, FoxNames, CurrentInheritedAbilityName)
	);
}

UTexture2D* USGameInstance::ConvertRenderTargetToTexture(UTextureRenderTarget2D* RenderTarget)
{
	if (!RenderTarget) return nullptr;

	UTexture2D* NewTexture = UKismetRenderingLibrary::RenderTargetCreateStaticTexture2DEditorOnly(
		RenderTarget,
		TEXT("SavedLineageTexture"),
		TextureCompressionSettings::TC_Default,
		TextureMipGenSettings::TMGS_NoMipmaps
	);

	return NewTexture;
}

void USGameInstance::LoadRenderPreviewTextures()
{
	for (FLineageCharacterData& Entry : PreviousLineageEntries)
	{
		Entry.RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, 225, 350);

		
	}
}

FName USGameInstance::GetCurrentPersonality() const
{
	return PreviousLineageEntries.Last().GetPersonality();
}

void USGameInstance::UpdateProgress()
{
	PrevProgess = CurrentProgress;
}

#pragma region Lineage Abilities Functions

// TODO: Store ability name and level
void USGameInstance::SetSkill(ASAbilityBase* Skill)
{
	SavedSkillName = Skill->GetAbilityName();

	PreviousLineageEntries.Last().SetSkillName(Skill->GetAbilityName());

	UE_LOG(LogTemp, Warning, TEXT("SetSkill: SavedSkillName: %s, CurrentLineageEntry.SkillName: %s"), *SavedSkillName, *PreviousLineageEntries.Last().GetSkillName());
}

void USGameInstance::SetSpell(ASAbilityBase* Spell)
{
	SavedSpellName = Spell->GetAbilityName();

	PreviousLineageEntries.Last().SetSpellName(Spell->GetAbilityName());
	UE_LOG(LogTemp, Warning, TEXT("SetSpell: SavedSpellName: %s, CurrentLineageEntry.SpellName: %s"), *SavedSpellName, *PreviousLineageEntries.Last().GetSpellName());
}

void USGameInstance::SetPassive(ASAbilityBase* Passive)
{
	SavedPassiveName = Passive->GetAbilityName();

	PreviousLineageEntries.Last().SetPassiveName(Passive->GetAbilityName());

	UE_LOG(LogTemp, Warning, TEXT("Setting Passive In this gameinstance"));
}

TSubclassOf<ASAbilityBase> USGameInstance::GetSkill()
{
	if (AbilityList.Contains(SavedSkillName))
	{
		return AbilityList[SavedSkillName];
	}
	return nullptr;
}

TSubclassOf<ASAbilityBase> USGameInstance::GetSpell()
{
	if (AbilityList.Contains(SavedSpellName))
	{
		return AbilityList[SavedSpellName];
	}
	return nullptr;
}

TSubclassOf<ASAbilityBase> USGameInstance::GetPassive()
{
	if (AbilityList.Contains(SavedPassiveName))
	{
		return AbilityList[SavedPassiveName];
	}
	return nullptr;
}

void USGameInstance::InheritAbility(ASAbilityBase* Ability)
{
	CurrentInheritedAbilityName = Ability->GetAbilityName();

	UE_LOG(LogTemp, Warning, TEXT("Inheriting Ability"));
}

TSubclassOf<AActor> USGameInstance::GetInheritedAbility()
{
	if (InteractableAbilityList.Contains(PreviousLineageEntries.Last().GetInheritedAbility()))
	{
		TSubclassOf<AActor> Ability = InteractableAbilityList[PreviousLineageEntries.Last().GetInheritedAbility()];

		PreviousLineageEntries.Last().SetInheritedAbility("");
		return Ability;
	}
	return nullptr;
}

//

void FLineageCharacterData::SetSkillName(FString NewSkillName)
{
	SkillName = NewSkillName;
}

void FLineageCharacterData::SetSpellName(FString NewSpellName)
{
	SpellName = NewSpellName;
}

void FLineageCharacterData::SetPassiveName(FString NewPassiveName)
{
	PassiveName = NewPassiveName;
}

void FLineageCharacterData::SetInheritedAbility(FString NewInheritedAbility)
{
	InheritedAbility = NewInheritedAbility;
}

//

#pragma endregion

#pragma region Fog Functions (Deprecated)

void USGameInstance::CreateFog()
{
	if (mFogClass == nullptr)
		return;

	FActorSpawnParameters spawnParam;
	for (int i = 0; i < FogRows; i++)
	{
		FogTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &USGameInstance::ProcessSpawningFog, (i * FogSpacing), 0, (i < CursedFogAmount || i > FogRows - CursedFogAmount) ? mCursedFogClass : mFogClass, spawnParam));
	}
}

void USGameInstance::ProcessSpawningFog(int rowPos, int columns, TSubclassOf<class ASFog> classToSpawn, const FActorSpawnParameters spawnParam)
{
	if (columns >= FogColumns)
	{
		return;
	}

	FVector spawnAdjustments = FVector(rowPos, (columns * FogSpacing), 200);
	FVector spawnPos = FogSpawnPosition + spawnAdjustments;

	ASFog* newFog = GetWorld()->SpawnActor<ASFog>((columns < CursedFogAmount || columns > FogColumns - CursedFogAmount) ? mCursedFogClass : classToSpawn, spawnPos, FRotator(0, 0, 0), spawnParam);

	columns++;
	FogTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &USGameInstance::ProcessSpawningFog, rowPos, columns, classToSpawn, spawnParam));
}

#pragma endregion

#pragma region Tutorial Functions

bool USGameInstance::HasSeenTutorial(UClass* ObjectClass) const
{
	if (!ObjectClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Has Seen Tutorial Object Is Null!"))
		return false;
	}

	if (!SeenTutorials.Contains(ObjectClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Seen Tutorials Map Does Not Contain Object Class: %s"), *ObjectClass->GetName())
		return false;
	}

	return SeenTutorials[ObjectClass];
}

void USGameInstance::MarkTutorialSeen(UClass* ObjectClass)
{
	if (!ObjectClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to mark tutorial as seen, but ObjectClass is null."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Marking Tutorial as Seen: %s"), *ObjectClass->GetName());

	if (SeenTutorials.Contains(ObjectClass))
	{
		SeenTutorials[ObjectClass] = true;
	}
}

void USGameInstance::SeenNPC(AActor* NPCActor)
{
	ASNPCInteraction* NPC = Cast<ASNPCInteraction>(NPCActor);
	if (!NPC) return;

	if (SeenNPCs.Contains(NPCActor)) return;

	UE_LOG(LogTemp, Warning, TEXT("Seen NPC Actor"));

	SeenNPCs.Add(NPCActor);
}

bool USGameInstance::HasSeenBothNPCs() const
{
	return SeenNPCs.Num() == 2;
}

#pragma endregion