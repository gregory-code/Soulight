// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/SCharacterBase.h"
#include "SPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, bool, bActionable);
/**
 *
 */

class USAbilityDataBase;
class USAbilityBase;

UCLASS()
class ASPlayer : public ASCharacterBase
{
	GENERATED_BODY()

public:
	ASPlayer();

public:
	FOnInteract OnInteract;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void GetGrabbed();

private:

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
	class UCameraComponent* MainCamera;

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
	USAbilityBase* CurrentSkill;

	UPROPERTY(EditAnywhere, Category = "Ability")
	USAbilityBase* CurrentSpell;

	UPROPERTY(EditAnywhere, Category = "Ability")
	USAbilityBase* CurrentPassive;

public:

	UFUNCTION()
	bool ObtainItem(USAbilityBase* newItem);

	UFUNCTION()
	EUpgrade GetItemStatus(USAbilityBase* newItem, USAbilityBase* currentItem);

	UFUNCTION()
	USAbilityBase* GetItemTypeFromNew(USAbilityBase* newItem);
};