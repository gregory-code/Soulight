// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/SGameInstance.h"
#include "SLineageCollection.generated.h"

class USLineageEntry;
/**
 * 
 */
UCLASS()
class USLineageCollection : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void CreateLineageCollection(TArray<FLineageCharacterData> CurrentLineage);

	void UpdateLatestLineageEntry();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* LineageScrollBox;

	class UTexture2D* GetAbilityIcon(FString AbilityName) const;

	TArray<USLineageEntry*> InstancedEntries;

	class USGameInstance* GameInstance;
};
