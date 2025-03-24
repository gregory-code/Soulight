// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Framework/SCharacterBase.h"
#include "Framework/SEquipmentData.h"

#include "SGameInstance.generated.h"

class ASAbilityBase;
class USEquipmentData;
class UTexture2D;
class ASLineageCharacterRenderPreview;

USTRUCT(BlueprintType)
struct FEquippedItems 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	USEquipmentData* EquippedWeapon = nullptr;

	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	USEquipmentData* EquippedChest = nullptr;

	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	USEquipmentData* EquippedHead = nullptr;

	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	USEquipmentData* EquippedBoot = nullptr;

	FEquippedItems()
		: EquippedWeapon(nullptr), EquippedChest(nullptr), EquippedHead(nullptr), EquippedBoot(nullptr)
	{}

	~FEquippedItems() = default;

	FEquippedItems(const FEquippedItems& Other) = default;

	FEquippedItems& operator=(const FEquippedItems& Other) = default;

	FEquippedItems(FEquippedItems&& Other) noexcept = default;

	FEquippedItems& operator=(FEquippedItems&& Other) noexcept = default;
};

/*
*	This is a lot of variables only because Unreal demands you use a custom
*	Struct in c++ in order to read the row data. Otherwise we could've just
*	used a structure in the editor.
*/
USTRUCT(BlueprintType)
struct FDialogueData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString Arrival_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString TalkToBlacksmith_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString TalkToFisher_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString TalkToMerchant_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString EnterForest_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString GrabbedByMistHand_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString LightsLampost_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString RevealsProwlerWithLight_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString HearsWendigo_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString SeesArborelHive_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString EnterCastle_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString SeesDreadstormFireball_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString OpensChest_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString TriggersMimic_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString SendsOffLetter_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString FindsSoul_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString KillsEnemy_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString EquipsMaxTierItem_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString ObtainsMaxTierAbility_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString SeesDreadstorm_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString Dies_Default;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString WinsGame_Default;
};

USTRUCT(BlueprintType)
struct FPersonalities : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString Adventurous;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString Ruthless;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString Paranoid;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString Stoic;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString Calm;
	UPROPERTY(EditAnywhere, Category = "Table Entry")
	FString Desperate;
};

USTRUCT(BlueprintType)
struct FLineageCharacterData
{
	GENERATED_BODY()

	// If you make a custom constructor in a struct, you MUST include the big 5!
	/*
	FGameplayAbilitySpec(const FGameplayAbilitySpec&) = default;
	FGameplayAbilitySpec(FGameplayAbilitySpec&&) = default;
	FGameplayAbilitySpec& operator=(const FGameplayAbilitySpec&) = default;
	FGameplayAbilitySpec& operator=(FGameplayAbilitySpec&&) = default;
	~FGameplayAbilitySpec() = default;
	*/

public:
	// Default constructor (Initializes members to default/null values)
	FLineageCharacterData()
		: RenderTarget(nullptr), 
		EquippedItems(),
		Personality(NAME_None),
		CharacterName(NAME_None),
		SkillName(TEXT("")), 
		SpellName(TEXT("")),
		PassiveName(TEXT("")),
		InheritedAbility(TEXT(""))
	{
		UE_LOG(LogTemp, Warning, TEXT("Default Lineage Entry Created"));
	}

	// Parameterized constructor
	FLineageCharacterData(TArray<FName> Personalities, TArray<FName> CharacterNames, FString NewInheritedAbility)
		: FLineageCharacterData()
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating Lineage Entry"));

		if (Personalities.Num() > 0)
		{
			int rand = FMath::RandRange(0, Personalities.Num() - 1);
			Personality = Personalities[rand];
		}

		if (CharacterNames.Num() > 0)
		{
			int rand = FMath::RandRange(0, CharacterNames.Num() - 1);
			CharacterName = CharacterNames[rand];
		}
	}

	// Destructor
	~FLineageCharacterData() = default;

	// Copy constructor
	FLineageCharacterData(const FLineageCharacterData& Other) = default;

	// Copy assignment operator
	FLineageCharacterData& operator=(const FLineageCharacterData& Other) = default;

	// Move constructor
	FLineageCharacterData(FLineageCharacterData&& Other) noexcept = default;

	// Move assignment operator
	FLineageCharacterData& operator=(FLineageCharacterData&& Other) noexcept = default;

	FName GetCharacterName() const { return CharacterName; }
	FName GetPersonality() const { return Personality; }
	FString GetSkillName() const { return SkillName; }
	FString GetSpellName() const { return SpellName; }
	FString GetPassiveName() const { return PassiveName; }
	FString GetInheritedAbility() const { return InheritedAbility; }

	void SetSkillName(FString NewSkillName);
	void SetSpellName(FString NewSpellName);
	void SetPassiveName(FString NewPassiveName);
	void SetInheritedAbility(FString NewInheritedAbility);

	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget = nullptr;

	FEquippedItems EquippedItems;

private:
	FName Personality;
	FName CharacterName;
	FString SkillName;
	FString SpellName;
	FString PassiveName;
	FString InheritedAbility;
};

/**
 * 
 */
UCLASS()
class USGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:

#pragma region Player Variables/Functions

	///////////////////////////////
	/*         Player           */
	/////////////////////////////

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	ESoulStatType CurrentSoulStatType;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FEquippedItems EquippedItems;

	void SetSkill(ASAbilityBase* Skill);
	void SetSpell(ASAbilityBase* Spell);
	void SetPassive(ASAbilityBase* Passive);
	
	TSubclassOf<ASAbilityBase> GetSkill();
	TSubclassOf<ASAbilityBase> GetSpell();
	TSubclassOf<ASAbilityBase> GetPassive();

	void ClearEquippedItems();
	class USSilentwingNoiseManager* GetNoiseManager();
	
private:
	UPROPERTY(EditAnywhere, Category = "Equipped Items")
	TMap<FString, TSubclassOf<ASAbilityBase>> AbilityList;

	UPROPERTY(EditDefaultsOnly, Category = "Equipped Items")
	TSubclassOf<USSilentwingNoiseManager> SilentwingNoiseManagerClass;
	
	UPROPERTY()
	class USSilentwingNoiseManager* SilentwingNoizeManager;

	FString InheritedAbilityName;
	int32 InheritedAbilityLevel;

	FString SavedSkillName;
	int32 SavedSkillLevel;

	FString SavedSpellName;
	int32 SavedSpellLevel;

	FString SavedPassiveName;
	int32 SavedPassiveLevel;

#pragma endregion

#pragma region Dungeon Variables/Functions

public:
	///////////////////////////////
	/*         Dungeon          */
	/////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	int32 CurrentProgress = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	int32 MaxProgress = 4;

	int32 PrevProgess = 0;

	void UpdateProgress();

#pragma endregion

#pragma region Fog Variables/Functions

	///////////////////////////////
	/*            Fog           */
	/////////////////////////////

	UFUNCTION()
	void CreateFog();

	UPROPERTY(EditDefaultsOnly, Category = "Fog")

	TSubclassOf<class ASFog> mFogClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	TSubclassOf<class ASFog> mCursedFogClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	FVector FogSpawnPosition;

	FTimerHandle FogTimerHandle;
	void ProcessSpawningFog(int rowPos, int columns, TSubclassOf<class ASFog> classToSpawn, const FActorSpawnParameters spawnParam);

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	int CursedFogAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	int FogRows;

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	int FogColumns;

	UPROPERTY(EditDefaultsOnly, Category = "Fog")
	int FogSpacing;

#pragma endregion 

#pragma region Player Variables/Functions

public:
	///////////////////////////////
	/*        Dialogue          */
	/////////////////////////////
	UFUNCTION(BlueprintCallable)
	FString GetResponse(const FName& Personality, const FString& InteractionName) const;

	FDialogueData* GetResponsesForPersonality() const;

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	class UDataTable* MyDataTable;

	//

	UFUNCTION(BlueprintCallable)
	bool HasBeenToSpiritsKeep() const { return bPlayerBeenToSpiritsKeep; }

	UFUNCTION(BlueprintCallable)
	void SetSpiritsKeepFlag(bool flag) { bPlayerBeenToSpiritsKeep = flag; }

	UFUNCTION(BlueprintCallable)
	bool GetGameFirstLoad() const { return bHasPlayerFirstStart; }

	UFUNCTION(BlueprintCallable)
	void SetGameFirstLoad() { bHasPlayerFirstStart = true; }

	//

	UFUNCTION(BlueprintCallable)
	void SetMinimap(USMinimap* Minimap);

	UFUNCTION(BlueprintCallable)
	USMinimap* GetMinimap() const { return MinimapInstance; }

private:
	bool bPlayerBeenToSpiritsKeep = false;
	bool bHasPlayerFirstStart = false;

	class USMinimap* MinimapInstance;

#pragma endregion

#pragma region Lineage Variables/Functions

public:
	UFUNCTION()
	void InitPreviewCharacter();

	UFUNCTION(BlueprintCallable)
	void StartLineage();

	UFUNCTION(BlueprintCallable)
	FName GetCurrentPersonality() const;

	void InheritAbility(ASAbilityBase* Ability);

	UFUNCTION(BlueprintCallable)
	const TArray<FLineageCharacterData>& GetCurrentLineage() const { return PreviousLineageEntries; }

	UFUNCTION()
	TSubclassOf<AActor> GetInheritedAbility();

	UFUNCTION()
	TSubclassOf<USLineageEntry> GetLineageEntryClass() const { return LineageEntryClass; }

	UPROPERTY(EditDefaultsOnly, Category = "Lineage")
	TMap<FString, class UTexture2D*> AbilityIcons;

	void LoadRenderPreviewTextures();
	void CaptureChararcterPreview();

	void SaveEquipment();
	void UpdateRenderPreviewEquipment(const FLineageCharacterData& Entry, const EEquipmentType& EquipmentType, UStaticMesh* StaticMesh);

	UPROPERTY(EditDefaultsOnly, Category = "Lineage")
	class UMaterialInterface* MI_CharacterPreviewRender;

private:
	UTexture2D* ConvertRenderTargetToTexture(UTextureRenderTarget2D* RenderTarget);

	UPROPERTY(EditDefaultsOnly, Category = "Lineage")
	TSubclassOf<ASLineageCharacterRenderPreview> LineageCharacterPreviewClass;

	ASLineageCharacterRenderPreview* LineageCharacterRenderPreview;

	TArray<FName> FoxNames = {
		"Amberpaw", "Blaze", "Rustfur", "Zinnia", "Foxtail", "Cinder", "Flameheart", "Sienna",
		"Duskswift", "Ashen", "Scarlet", "Solara", "Emberwhisk", "Copperfang", "Vixen", "Bramble",
		"Tawny", "Aurora", "Skye", "Shadowflare", "Fenn", "Wildfire", "Ryn", "Fenrir", "Frostpaw",
		"Maple", "Rowan", "Citrine", "Dawnrunner", "Sunflare", "Blazeclaw", "Poppy", "Kestrel"
	};

	UPROPERTY()
	FString CurrentInheritedAbilityName;

	UPROPERTY(EditDefaultsOnly, Category = "Lineage")
	TArray<FName> Personalities;

	UPROPERTY(EditAnywhere, Category = "Lineage")
	TMap<FString, TSubclassOf<AActor>> InteractableAbilityList;

	UPROPERTY(EditDefaultsOnly, Category = "Lineage")
	TArray<FString> Abilities;

	UPROPERTY()
	TArray<FLineageCharacterData> PreviousLineageEntries;

	UPROPERTY(EditDefaultsOnly, Category = "Lineage")
	TSubclassOf<USLineageEntry> LineageEntryClass;

#pragma endregion

#pragma region Tutorial Variables/Functions

public:
	bool HasSeenTutorial(UClass* ObjectClass) const;
	void MarkTutorialSeen(UClass* ObjectClass);

	void SeenNPC(AActor* NPCActor);

	UFUNCTION(BlueprintCallable)
	bool HasSeenBothNPCs() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Tutorial")
	TMap<UClass*, bool> SeenTutorials;

	TArray<AActor*> SeenNPCs;

#pragma endregion

};
