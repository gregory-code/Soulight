// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SCharacterBase.h"
#include "SPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
/**
 *
 */

class ASAbilityBase;
class USEquipmentData;
class USAbilityDataBase;
class USStatData;
class USoundBase;

UCLASS()
class ASPlayer : public ASCharacterBase
{
	GENERATED_BODY()

public:
	ASPlayer();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* MainCamera;

	FOnInteract OnInteract;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void GetGrabbed();

	UFUNCTION()
	void EndCombo();

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TArray<UAnimMontage*> ComboSectionMontages;

	UPROPERTY()
	int32 CurrentCombo = 0;

	UPROPERTY()
	bool bCanAttack = true;
	UPROPERTY()
	bool bHasAttacked = false;

	UFUNCTION()
	bool GetIsDead() const { return bIsDead; }


#pragma region SFX Variables/Functions

private:
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* HitSound;

#pragma endregion

#pragma region Dialogue Variables/Functions

private:
	///////////////////////////////
	/*         Dialogue         */
	/////////////////////////////
	UFUNCTION(BlueprintCallable)
	void Speak(const FString& Dialogue);
	UFUNCTION(BlueprintCallable)
	void ClearSpeakText();

	UFUNCTION()
	void IntroSpeak();

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	class UWidgetComponent* WidgetComponent;

#pragma endregion

#pragma region Equipment Variables/Functions

private:
	UFUNCTION()
	void UpdateEquippedIfAny();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Customization")
	UStaticMeshComponent* HeadMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Customization")
	FName HeadSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Customization")
	UStaticMeshComponent* ChestMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Customization")
	FName ChestSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Customization")
	UStaticMeshComponent* BootsMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Customization")
	FName BootsSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Customization")
	UStaticMeshComponent* WeaponMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Customization")
	FName WeaponSocketName;

	UFUNCTION()
	void WearItem(EEquipmentType EquipmentType, UStaticMesh* StaticMesh);

public:
	///////////////////////////////
	/*        Equipment         */
	/////////////////////////////

	void EquipItem(USEquipmentData* EquipmentData);

private:
	UPROPERTY()
	USEquipmentData* WeaponEquipmentData;

	UPROPERTY()
	USEquipmentData* ChestEquipmentData;
	UPROPERTY()
	USEquipmentData* HeadEquipmentData;
	UPROPERTY()
	USEquipmentData* BootEquipmentData;


#pragma endregion

#pragma region Health/Damage Variables/Functions

public:
	virtual void TakeDamage(float Damage, AActor* Instigator, const float& Knockback) override;

private:
	UFUNCTION()
	void StartDeath(bool IsDead);
	UFUNCTION()
	void HealthUpdated(const float newHealth);

	UFUNCTION()
	void RegenerateHealth();
	UFUNCTION()
	void StartHealthRegen();

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float RegenAmount = 1.0f;

	FTimerHandle HealthRegenTimerHandle;
	FTimerHandle RegenDelayTimerHandle;

#pragma endregion

private:
	UFUNCTION()
	void LoadSpiritsKeep();

	bool bIsDead = false;

#pragma region Lamp Light Variables/Functions

public:
	UFUNCTION()
	void SetLampLightColor(const FLinearColor& NewLampColor);

	UFUNCTION()
	void SetSoulBuff(const ESoulStatType& StatType);

private:
	///////////////////////////////
	/*          Lamp            */
	/////////////////////////////

	// Idk what to name this
	UPROPERTY(EditDefaultsOnly, Category = "Light")
	TMap<ESoulStatType, FSoulMultiplier> SoulBuff;

	UPROPERTY(EditAnywhere, Category = "Light")
	class UPointLightComponent* LampLight;

	// This is the one actually lighting things since the lamp light is too close
	UPROPERTY(EditAnywhere, Category = "Light")
	class UPointLightComponent* VisualLight;

	UPROPERTY(EditAnywhere, Category = "Light")
	float LightIntensity = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Light")
	FName LampSocket;

	UPROPERTY(EditAnywhere, Category = "Light")
	FLinearColor BaseLampColor;

#pragma endregion

#pragma region Input Variables/Functions

private:
	///////////////////////////////
	/*         Inputs           */
	/////////////////////////////
	
	void SetInputMapping(bool bPlayerMapping);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* InputPlayerMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* InputInteractionMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* AimInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* InteractInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* AttackInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* DodgeInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SkillInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SpellInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* HUDInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SettingsInputAction;

	UFUNCTION()
	void Move(const FInputActionValue& InputValue);

	UFUNCTION()
	void Aim(const FInputActionValue& InputValue);

	UFUNCTION()
	void Interact();

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void Dodge();

	UFUNCTION()
	void Skill();

	UFUNCTION()
	void Spell();

	UFUNCTION()
	void HUD();
	bool bHUDEnabled;

	UFUNCTION()
	void Settings();

	///////////////////////////////
	/*         Attack           */
	/////////////////////////////

	UFUNCTION()
	void AttackCombo();

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* AttackComboMontage;


#pragma endregion

#pragma region Camera Variables/Functions

public:
	/*
	*	Using Blueprint Events here as running the Camera Manager code in
	*	c++ is wierd
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "VFX")
	void CameraShake_BlueprintEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "VFX")
	void CameraFade_BlueprintEvent(const float& Duration);

private:
	///////////////////////////////
	/*         Camera           */
	/////////////////////////////

	UPROPERTY(visibleAnywhere, Category = "Camera")
	class USceneComponent* MainCameraPivot;

	UPROPERTY(visibleAnywhere, Category = "Camera")
	class USceneComponent* FullHealthView;

	UPROPERTY(visibleAnywhere, Category = "Camera")
	class USceneComponent* EmptyHealthView;

	UPROPERTY(visibleAnywhere, Category = "Camera")
	class USceneCaptureComponent2D* MiniMapCamera;

	UPROPERTY(visibleAnywhere, Category = "Camera")
	class USceneComponent* MiniMapView;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UStaticMeshComponent* MinimapPlayerIcon;

	FTimerHandle CameraTimerHandle;
	float CameraMoveSpeed = 5.0f;
	void MoveCameraToLocalOffset(const FVector& LocalOffset);
	void ProcessCameraMove(FVector Goal);

#pragma endregion

private:
	bool bZoomOut = false;
	bool bInteractOnly;
	bool bGrabbedInactionable;
	
	class USGameInstance* GameInstance;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	class ASPlayerController* PlayerController;

	virtual void PawnClientRestart() override;

	UPROPERTY(EditDefaultsOnly, Category = "Testing")
	ESoulStatType CurrentSoulBuff;

	///////////////////////////////
	/*       Fog Comp           */
	/////////////////////////////

	UPROPERTY(EditAnywhere, Category = "Fog Cleaner")
	TSubclassOf<class ASFogCleaner> mFogCleanerClass;

	UPROPERTY(visibleAnywhere, Category = "Fog Cleaner")
	class ASFogCleaner* FogCleaner;

#pragma region Helper Variables/Functions

private:

	///////////////////////////////
	/*       Movement           */
	/////////////////////////////

	FVector GetMoveFwdDir() const;
	FVector GetMoveRightDir() const;
	FVector previousDir;

#pragma endregion

#pragma region Ability Variables/Functions

public:
	///////////////////////////////
	/*        Abilities         */
	/////////////////////////////

	UFUNCTION()
	bool ObtainItem(ASAbilityBase* newItem);

	UFUNCTION()
	EUpgrade GetItemStatus(ASAbilityBase* newItem, ASAbilityBase* currentItem);

	UFUNCTION()
	void SetNewAbility(ASAbilityBase* newItem, USAbilityDataBase* NewAbilityData);

	UFUNCTION()
	ASAbilityBase* GetItemTypeFromNew(ASAbilityBase* newItem);

	UFUNCTION()
	ASAbilityBase* GetCurrentSkill() const { return CurrentSkill; }

	UFUNCTION()
	ASAbilityBase* GetCurrentSpell() const { return CurrentSpell; }

	UFUNCTION()
	ASAbilityBase* GetCurrentPassive() const { return CurrentPassive; }

private:
	UPROPERTY(EditAnywhere, Category = "Ability")
	ASAbilityBase* CurrentSkill;

	UPROPERTY(EditAnywhere, Category = "Ability")
	ASAbilityBase* CurrentSpell;

	UPROPERTY(EditAnywhere, Category = "Ability")
	ASAbilityBase* CurrentPassive;


#pragma endregion

#pragma region Debug Variables/Functions

private:
	void DEBUG_ModifyHealth(const FInputActionValue& InputValue);
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ModifyHealthInputAction;


#pragma endregion

};