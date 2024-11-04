// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SCharacterBase.h"
#include "SPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
/**
 *
 */

class USEquipmentData;
class USAbilityDataBase;
class ASAbilityBase;
class USStatData;

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

	virtual void TakeDamage(float Damage) override;

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

private:
	UFUNCTION()
	void StartDeath(bool IsDead);

	bool bIsDead = false;

	void DEBUG_ModifyHealth(const FInputActionValue& InputValue);
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ModifyHealthInputAction;


private:
	bool bZoomOut = false;

	UPROPERTY(EditAnywhere, Category = "Light")
	class UPointLightComponent* LampLight;

	// This is the one actually lighting things since the lamp light is too close
	UPROPERTY(EditAnywhere, Category = "Light")
	class UPointLightComponent* VisualLight;

	UPROPERTY(EditAnywhere, Category = "Light")
	float LightIntensity = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Light")
	FName LampSocket;


	UPROPERTY(VisibleAnywhere, Category = "Input")
	class ASPlayerController* PlayerController;

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

	virtual void PawnClientRestart() override;

	///////////////////////////////
	/*         Inputs           */
	/////////////////////////////

	bool bInteractOnly;
	bool bGrabbedInactionable;

	void SetInputMapping(bool bPlayerMapping);

	UPROPERTY(EditDefaultsOnly, Category = "Testing Stuff")
	AActor* Testing;

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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	
	///////////////////////////////
	/*       Movement           */
	/////////////////////////////

	FVector GetMoveFwdDir() const;
	FVector GetMoveRightDir() const;
	FVector previousDir;

	///////////////////////////////
	/*       Health             */
	/////////////////////////////

	UFUNCTION()
	void HealthUpdated(const float newHealth);

	///////////////////////////////
	/*       Fog Comp           */
	/////////////////////////////

	UPROPERTY(EditAnywhere, Category = "Fog Cleaner")
	TSubclassOf<class ASFogCleaner> mFogCleanerClass;

	UPROPERTY(visibleAnywhere, Category = "Fog Cleaner")
	class ASFogCleaner* FogCleaner;

	///////////////////////////////
	/*         Skills           */
	/////////////////////////////

	UPROPERTY(EditAnywhere, Category = "Ability")
	ASAbilityBase* CurrentSkill;

	UPROPERTY(EditAnywhere, Category = "Ability")
	ASAbilityBase* CurrentSpell;

	UPROPERTY(EditAnywhere, Category = "Ability")
	ASAbilityBase* CurrentPassive;

public:
	///////////////////////////////
	/*        Equipment         */
	/////////////////////////////

	void EquipItem(USEquipmentData* EquipmentData);

private:
	USEquipmentData* WeaponItemStats;

	USEquipmentData* ChestItemStats;
	USEquipmentData* HeadItemStats;
	USEquipmentData* BootItemStats;

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
};