// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/SPlayerController.h"
#include "InputCoreTypes.h"
#include "GameFramework/PlayerInput.h"

#include "AI/SSilentwingNoiseManager.h"

#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SDialogueHandlerComponent.h"
#include "Components/STutorialComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Engine/StaticMesh.h"

#include "Enemies/SEnemy.h"

#include "Curves/CurveFloat.h"

#include "Abilities/SAbilityBase.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Framework/SFogCleaner.h"
#include "Framework/SEquipmentData.h"
#include "Framework/SGameInstance.h"
#include "Framework/SInteractableObject.h"
#include "Framework/SItemBase.h"

#include "Player/Abilities/SAbilityDataBase.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneCaptureComponent2D.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Interactable/IInteractable.h"
#include "Interactable/SEquipment.h"

#include "UObject/ConstructorHelpers.h"

#include "Widgets/STextPrompt.h"
#include "Widgets/SPlayerHUDUI.h"
#include "Widgets/SLineageCollection.h"

ASPlayer::ASPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	MainCameraPivot = CreateDefaultSubobject<USceneComponent>("Pivot of Main Camera");
	FullHealthView = CreateDefaultSubobject<USceneComponent>("Full Health Pos");
	EmptyHealthView = CreateDefaultSubobject<USceneComponent>("Empty Health Pos");
	MainCamera = CreateDefaultSubobject<UCameraComponent>("Main Camera");
	LampLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Lamp Light"));
	VisualLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Visual Light"));

	VisualLight->SetupAttachment(GetRootComponent());

	FullHealthView->SetupAttachment(GetRootComponent());
	FullHealthView->SetWorldLocation(FVector(-600, 0, 1000));

	EmptyHealthView->SetupAttachment(GetRootComponent());
	EmptyHealthView->SetWorldLocation(FVector(-300, 0, 400));

	MainCamera->SetupAttachment(MainCameraPivot, USpringArmComponent::SocketName);
	MainCamera->SetWorldLocation(FullHealthView->GetRelativeLocation());
	MainCamera->SetWorldRotation(FRotator(0, -62.0f, 0));

	MiniMapView = CreateDefaultSubobject<USceneComponent>("Mini Map Pos");
	MiniMapView->SetupAttachment(GetRootComponent());
	MiniMapView->SetWorldLocation(FVector(0, 0, 1500.0));

	MiniMapCamera = CreateDefaultSubobject<USceneCaptureComponent2D>("Mini Map Camera");
	MiniMapCamera->SetWorldRotation(FRotator(0, -90.0f, 0));

	DialogueHandlerComponent = CreateDefaultSubobject<USDialogueHandlerComponent>("Dialogue Handler Component");
	TutorialComponent = CreateDefaultSubobject<USTutorialComponent>("Tutorial Component");

	MinimapPlayerIcon = CreateDefaultSubobject<UStaticMeshComponent>("Minimap Player Icon");
	MinimapPlayerIcon->CastShadow = false;
	MinimapPlayerIcon->SetupAttachment(GetRootComponent());
	MinimapPlayerIcon->SetAutoActivate(true);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (PlaneMeshAsset.Succeeded())
	{
		MinimapPlayerIcon->SetStaticMesh(PlaneMeshAsset.Object);
		MinimapPlayerIcon->SetWorldLocation(FVector(15, 0, 1000.0));
		MinimapPlayerIcon->SetWorldRotation(FRotator(0, 0, 90.0f));
		MinimapPlayerIcon->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	if (IsValid(GetMesh())) 
	{
		HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>("Head Mesh");
		ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>("Chest Mesh");
		BootsMesh = CreateDefaultSubobject<UStaticMeshComponent>("Boots Mesh");
		WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Mesh");
	}

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interact Sphere"));
	InteractSphere->SetupAttachment(GetRootComponent());

	TutorialOverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Tutorial Overlap Sphere"));
	TutorialOverlapSphere->SetupAttachment(GetRootComponent());

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->SetupAttachment(GetRootComponent());

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(1080.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
}

void ASPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MiniMapCamera->SetRelativeLocation(MiniMapView->GetComponentLocation());
	MainCameraPivot->SetRelativeLocation(GetActorLocation());

	if (!bIsUsingGamepad) // If using Mouse
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			FVector WorldLocation, WorldDirection;
			PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

			// Plane Intersection (Find where the ray hits the ground)
			FVector PlayerLocation = GetActorLocation();
			FVector PlaneNormal = FVector(0.0f, 0.0f, 1.0f); // Z-Up normal for ground plane
			float Distance = (PlayerLocation.Z - WorldLocation.Z) / WorldDirection.Z;
			FVector TargetLocation = WorldLocation + WorldDirection * Distance;

			// Make player face the mouse
			FRotator LookAtRotation = (TargetLocation - PlayerLocation).Rotation();
			SetActorRotation(FRotator(0.0f, LookAtRotation.Yaw, 0.0f));
		}
	}

	if (FogCleaner)
	{
		FogCleaner->SetActorRelativeLocation(GetActorLocation());
	}
}

void ASPlayer::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<USGameInstance>(GetGameInstance());

	FTimerHandle EquipItemTimer;
	GetWorld()->GetTimerManager().SetTimer(EquipItemTimer, this, &ASPlayer::UpdateEquippedIfAny, 1.0f, false, 0.2f);

	if (GameInstance) 
	{
		GameInstance->InitPreviewCharacter();

		if (!GameInstance->HasBeenToSpiritsKeep()) // Game First Load
		{
			GameInstance->StartLineage();

			GameInstance->SetSpiritsKeepFlag(true);

			FTimerHandle SpeechDelayTimer;
			GetWorld()->GetTimerManager().SetTimer(SpeechDelayTimer, this, &ASPlayer::IntroSpeak, 1.0f, false, 0.2f);
		}

		GameInstance->LoadRenderPreviewTextures();
	}

	// TODO : FIX THIS
	// Material and render target are becoming null since pointer gets deleted on scene change
	PlayerController->GetPlayerHUD()->CreateLineage();

	UpdateEquippedIfAny();

	if(IsValid(HeadMesh))
		HeadMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HeadSocketName);
	if (IsValid(ChestMesh))
		ChestMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, ChestSocketName);
	if (IsValid(BootsMesh))
		BootsMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, BootsSocketName);
	if (IsValid(WeaponMesh))
		WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);

	HUDInputAction->bTriggerWhenPaused = true;
	SettingsInputAction->bTriggerWhenPaused = true;

	if (IsValid(GetMesh()) && IsValid(LampLight))
	{
		if (GetMesh()->DoesSocketExist(LampSocket))
		{
			LampLight->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LampSocket);
			SetLampLightColor(BaseLampColor);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Socket %s does not exist on the skeletal mesh!"), *LampSocket.ToString());
		}
	}
	FInputModeGameOnly input;
	GetWorld()->GetFirstPlayerController()->SetInputMode(input);

	FActorSpawnParameters spawnParam;
	FVector spawnPos = GetActorLocation();

	FogCleaner = GetWorld()->SpawnActor<ASFogCleaner>(mFogCleanerClass, spawnPos, FRotator(0, 0, 0), spawnParam);

	OnDead.AddDynamic(this, &ASPlayer::StartDeath);

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeedCurve->GetFloatValue(Agility);

	if(GameInstance)
		SetSoulBuff(GameInstance->CurrentSoulStatType);

	if (IsValid(PlayerController))
	{
		PlayerController->SetStatsUI(Strength, Defense, Agility, Soul);

	}

	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &ASPlayer::OnBeginOverlap);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &ASPlayer::OnEndOverlap);

	TutorialOverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ASPlayer::OnTutorialSphereBeginOverlap);
	TutorialOverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ASPlayer::OnTutorialSphereEndOverlap);

	if (bUseSceneComponentsHealthPositionOnStart)
	{
		FullHealthCameraPosition = FullHealthView->GetRelativeLocation();
		EmptyHealthCameraPosition = EmptyHealthView->GetRelativeLocation();
	}

	HealthUpdated(1.f);

	LampLight->SetIntensity(LightIntensity);
}

void ASPlayer::SetUseCustomHealthPosition(bool UseCustomPosition)
{
	bUseSceneComponentsHealthPositionOnStart = UseCustomPosition;
}

void ASPlayer::SetFullHealthPosition(const FVector& NewPosition)
{
	FullHealthCameraPosition = NewPosition;

	HealthUpdated(Health/MaxHealth);
}

void ASPlayer::SetEmptyHealthPosition(const FVector& NewPosition)
{
	EmptyHealthCameraPosition = NewPosition;

	HealthUpdated(Health / MaxHealth);
}

void ASPlayer::PawnClientRestart()
{
	Super::PawnClientRestart();

	PlayerController = GetController<ASPlayerController>();
	SetInputMapping(true);
}

#pragma region Dialogue Functions

void ASPlayer::Speak(const FString& Dialogue)
{
	if (!IsValid(WidgetComponent)) return;
	if (!IsValid(WidgetComponent->GetWidget())) return;

	USTextPrompt* TextPrompt = Cast<USTextPrompt>(WidgetComponent->GetWidget());
	if (!IsValid(TextPrompt)) return;

	const FText Text = FText::FromString(Dialogue);
	TextPrompt->SetText(Text);

	if (GetWorld()->GetTimerManager().IsTimerPending(ClearSpeakHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ClearSpeakHandle);
	}

	GetWorld()->GetTimerManager().SetTimer(ClearSpeakHandle, this, &ASPlayer::ClearSpeakText, 1.0f, false, 2.5f);
}

void ASPlayer::ClearSpeakText()
{
	if (!IsValid(WidgetComponent)) return;
	if (!IsValid(WidgetComponent->GetWidget())) return;

	USTextPrompt* TextPrompt = Cast<USTextPrompt>(WidgetComponent->GetWidget());
	if (!IsValid(TextPrompt)) return;

	const FText Text = FText::FromString("");
	TextPrompt->SetText(Text);
}

void ASPlayer::IntroSpeak()
{
	FDialogueData* Responses = GameInstance->GetResponsesForPersonality();
	Speak(Responses->Arrival_Default);
}

#pragma endregion

#pragma region Equipment Functions

/*
*	This looks pretty ugly but it just reads the struct from the Game Instance
*	then equips the any ability/equipment that was previously owned from another scene.
*/
void ASPlayer::UpdateEquippedIfAny()
{
	if (!IsValid(GameInstance)) return;

	TSubclassOf<ASAbilityBase> SkillClass = GameInstance->GetSkill();
	TSubclassOf<ASAbilityBase> SpellClass = GameInstance->GetSpell();
	TSubclassOf<ASAbilityBase> PassiveClass = GameInstance->GetPassive();

	if (IsValid(SkillClass))
	{
		ASAbilityBase* NewAbility = GetWorld()->SpawnActor<ASAbilityBase>(SkillClass);
		if (IsValid(NewAbility))
		{
			ObtainItem(NewAbility);
			UE_LOG(LogTemp, Warning, TEXT("I am assigning the GameInstance Skill"));
			NewAbility->RegisterAbility(this);
		}
	}

	if (IsValid(SpellClass))
	{
		ASAbilityBase* NewAbility = GetWorld()->SpawnActor<ASAbilityBase>(SpellClass);
		if (IsValid(NewAbility))
		{
			ObtainItem(NewAbility);
			UE_LOG(LogTemp, Warning, TEXT("I am assigning the GameInstance Spell"));
			NewAbility->RegisterAbility(this);
		}
	}

	if (IsValid(PassiveClass))
	{
		ASAbilityBase* NewAbility = GetWorld()->SpawnActor<ASAbilityBase>(PassiveClass);
		if (IsValid(NewAbility))
		{
			ObtainItem(NewAbility);
			UE_LOG(LogTemp, Warning, TEXT("I am assigning the GameInstance Passive"));
			NewAbility->RegisterAbility(this);
		}
	}

	WeaponEquipmentData = GameInstance->EquippedItems.EquippedWeapon;
	if (IsValid(WeaponEquipmentData))
	{
		AddStats(WeaponEquipmentData->EquipmentStats);
		WearItem(WeaponEquipmentData->EquipmentType, WeaponEquipmentData->EquipmentMesh);

		PlayerController->GetPlayerHUD()->UpdateEquipmentIcon(WeaponEquipmentData->EquipmentIcon, EEquipmentType::WEAPON);
		//WearItem(EEquipmentType::WEAPON, WeaponEquipmentData->EquipmentMesh);
	}
	else UE_LOG(LogTemp, Warning, TEXT("Weapon Is null"));

	ChestEquipmentData = GameInstance->EquippedItems.EquippedChest;
	if (IsValid(ChestEquipmentData))
	{
		AddStats(ChestEquipmentData->EquipmentStats);
		WearItem(ChestEquipmentData->EquipmentType, ChestEquipmentData->EquipmentMesh);

		PlayerController->GetPlayerHUD()->UpdateEquipmentIcon(ChestEquipmentData->EquipmentIcon, EEquipmentType::CHEST);
	}
	else UE_LOG(LogTemp, Warning, TEXT("Chest Is null"));

	HeadEquipmentData = GameInstance->EquippedItems.EquippedHead;
	if (IsValid(HeadEquipmentData))
	{
		AddStats(HeadEquipmentData->EquipmentStats);
		WearItem(HeadEquipmentData->EquipmentType, HeadEquipmentData->EquipmentMesh);

		PlayerController->GetPlayerHUD()->UpdateEquipmentIcon(HeadEquipmentData->EquipmentIcon, EEquipmentType::HEAD);
	}
	else UE_LOG(LogTemp, Warning, TEXT("Head Is null"));

	BootEquipmentData = GameInstance->EquippedItems.EquippedBoot;
	if (IsValid(BootEquipmentData))
	{
		AddStats(BootEquipmentData->EquipmentStats);
		WearItem(BootEquipmentData->EquipmentType, BootEquipmentData->EquipmentMesh);

		PlayerController->GetPlayerHUD()->UpdateEquipmentIcon(BootEquipmentData->EquipmentIcon, EEquipmentType::BOOTS);
	}
	else UE_LOG(LogTemp, Warning, TEXT("Boots Is null"));
}

/*
*	This sets the mesh that is attached to the socket on the player
*/
void ASPlayer::WearItem(EEquipmentType EquipmentType, UStaticMesh* StaticMesh)
{
	if (!IsValid(StaticMesh)) return;

	switch (EquipmentType)
	{
		case EEquipmentType::WEAPON:
			WeaponMesh->SetStaticMesh(StaticMesh);
			break;
		case EEquipmentType::CHEST:
			ChestMesh->SetStaticMesh(StaticMesh);
			break;
		case EEquipmentType::HEAD:
			HeadMesh->SetStaticMesh(StaticMesh);
			break;
		case EEquipmentType::BOOTS:
			BootsMesh->SetStaticMesh(StaticMesh);
			break;
	}

	GameInstance->UpdateRenderPreviewEquipment(GameInstance->GetCurrentLineage().Last(), EquipmentType, StaticMesh);
}

#pragma endregion

#pragma region Input Functions

void ASPlayer::SetInputMapping(bool bPlayerMapping)
{
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* inputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		inputSystem->ClearAllMappings();
		inputSystem->AddMappingContext((bPlayerMapping) ? InputPlayerMapping : InputInteractionMapping, 0); // terinary if/else condition
	}
}

void ASPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent)
	{
		enhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ASPlayer::Move);
		enhancedInputComponent->BindAction(AimInputAction, ETriggerEvent::Triggered, this, &ASPlayer::Aim);
		enhancedInputComponent->BindAction(AttackInputAction, ETriggerEvent::Started, this, &ASPlayer::Attack);
		enhancedInputComponent->BindAction(DodgeInputAction, ETriggerEvent::Started, this, &ASPlayer::Dodge);
		enhancedInputComponent->BindAction(SkillInputAction, ETriggerEvent::Started, this, &ASPlayer::Skill);
		enhancedInputComponent->BindAction(SpellInputAction, ETriggerEvent::Started, this, &ASPlayer::Spell);
		enhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Started, this, &ASPlayer::Interact);
		enhancedInputComponent->BindAction(HUDInputAction, ETriggerEvent::Started, this, &ASPlayer::HUD);
		enhancedInputComponent->BindAction(SettingsInputAction, ETriggerEvent::Started, this, &ASPlayer::Settings);
		enhancedInputComponent->BindAction(ModifyHealthInputAction, ETriggerEvent::Triggered, this, &ASPlayer::DEBUG_ModifyHealth);
	}
}

void ASPlayer::Move(const FInputActionValue& InputValue)
{
	if (bIsDead) return;

	FVector2D input = InputValue.Get<FVector2D>();
	input.Normalize();

	if (previousDir != FVector(0.0f, 0.0f, 0.0f))
	{
		previousDir = FVector(0.0f, 0.0f, 0.0f);
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	AddMovementInput(input.Y * GetMoveFwdDir() + input.X * GetMoveRightDir());
}

void ASPlayer::Aim(const FInputActionValue& InputValue)
{
	if (bIsDead) return;

	if (!bIsUsingGamepad) return;

	FVector2D input = InputValue.Get<FVector2D>();
	input.Normalize();

	GetCharacterMovement()->bOrientRotationToMovement = false;

	previousDir = FVector(input.X, input.Y, 0.0f);

	SetActorRotation(FVector(input.X, input.Y, 0.0f).ToOrientationRotator());
}

void ASPlayer::Interact()
{
	if (bIsDead) return;

	if (GetClosestInteractable())
	{
		if (GetClosestInteractable()->GetClass()->ImplementsInterface(UIInteractable::StaticClass()))
		{
			ASInteractableObject* InteractedObject = Cast<ASInteractableObject>(GetClosestInteractable());
			if (IsValid(InteractedObject))
			{
				UE_LOG(LogTemp, Warning, TEXT("Interact Object Cast Success"));

				ASItemBase* InteractedAbility = Cast<ASItemBase>(InteractedObject);
				if (IsValid(InteractedAbility))
				{
					UE_LOG(LogTemp, Warning, TEXT("Successfully Broadcast Ability Interact"));

					OnPickup.Broadcast(InteractedAbility->GetAbilityIcon(), InteractedAbility->GetAbilityName());
				}

				ASEquipment* InteractedEquipment = Cast<ASEquipment>(GetClosestInteractable());
				if (IsValid(InteractedEquipment))
				{
					if (IsValid(InteractedEquipment->GetEquipmentData()))
					{
						OnPickupEquipment.Broadcast(InteractedEquipment->GetEquipmentData()->EquipmentIcon, InteractedEquipment->GetEquipmentData()->EquipmentName);
					}
				}

				//OnPickup.Broadcast(InteractedObject);
			}

			OnInteract.Broadcast(GetClosestInteractable());

			IIInteractable::Execute_Interact(GetClosestInteractable(), this);
		}
	}
}

void ASPlayer::Attack()
{
	if (bIsDead) return;

	if (bCanAttack == false) 
	{
		UE_LOG(LogTemp, Warning, TEXT("I Cannot Attack"))
		return;
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("I Can Attack"))
	}

	//if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) return;

	if (IsValid(CurrentSkill) && CurrentSkill->GetAbilityActive())
	{
		CurrentSkill->CancelAbility();

		EndCombo();

		return;
	}

	if (IsValid(CurrentSpell) && CurrentSpell->GetAbilityActive())
	{
		CurrentSpell->CancelAbility();

		EndCombo();

		return;
	}

	AttackCombo(); // Error here, Delegate, null exception

	UGameplayStatics::PlaySound2D(this, AttackSound);
}

void ASPlayer::Dodge()
{
	if (bIsDead) return;

	//HealthUpdated(0.5f);
}

void ASPlayer::Skill()
{
	if (bIsDead) return;

	if (!IsValid(CurrentSkill)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Slot Is Null"));

		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Skill"));

	CurrentSkill->ExecuteAbility();
}

void ASPlayer::Spell()
{
	if (bIsDead) return;

	if (!IsValid(CurrentSpell)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Spell Slot Is Null"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Spell"));

	CurrentSpell->ExecuteAbility();
}

void ASPlayer::HUD()
{
	if (!PlayerController) return;

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (bHUDEnabled)
	{
		// Resume Game
		bHUDEnabled = false;
		PlayerController->SetShowMouseCursor(false);

		UE_LOG(LogTemp, Warning, TEXT("Unpausing"));

		// Remove UI Input, Add Gameplay Input
		InputSubsystem->RemoveMappingContext(InputInteractionMapping);
		InputSubsystem->AddMappingContext(InputPlayerMapping, 0);
	}
	else
	{
		// Pause Game
		bHUDEnabled = true;
		PlayerController->SetShowMouseCursor(true);

		UE_LOG(LogTemp, Warning, TEXT("Pausing"));

		// Remove Gameplay Input, Add UI Input
		InputSubsystem->RemoveMappingContext(InputPlayerMapping);
		InputSubsystem->AddMappingContext(InputInteractionMapping, 0);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Current Active Mapping Context: %s"), *InputSubsystem->Map()->GetName());

	PlayerController->GameplayUIState(bHUDEnabled);
}



void ASPlayer::Settings()
{

}

void ASPlayer::SetUsingGamepad(bool IsUsingGamepad)
{
	bIsUsingGamepad = IsUsingGamepad;
}

void ASPlayer::AttackCombo()
{
	if (!IsValid(AttackComboMontage)) return;

	bCanAttack = false;
	bHasAttacked = true;

	UE_LOG(LogTemp, Warning, TEXT("Section: %d"), CurrentCombo);

	if (!IsValid(GetMesh()->GetAnimInstance())) return;

	//GetMesh()->GetAnimInstance()->StopAllMontages(1.0f);

	GetMesh()->GetAnimInstance()->Montage_Play(ComboSectionMontages[CurrentCombo], AttackSpeedCurve->GetFloatValue(Agility));

	USSilentwingNoiseManager::Get(this).GenerateNoise(10.f, GetActorLocation());
}

void ASPlayer::EndCombo()
{
	bHasAttacked = false;
	bCanAttack = true;
	CurrentCombo = 0;
}

#pragma endregion

#pragma region Health/Damage Functions

void ASPlayer::CharacterTakeDamage(float Damage, AActor* DamageInstigator, const float& Knockback)
{
	if (bIsDead) return;

	CameraShake_BlueprintEvent();

	Super::CharacterTakeDamage(Damage, DamageInstigator, Knockback);

	if (GetIsDamageable() == false) return;

	EndCombo();

	if (IsValid(CurrentSkill))
		CurrentSkill->EndAbility();

	if (IsValid(CurrentSpell))
		CurrentSpell->EndAbility();

	float NewHealth = (Health / MaxHealth);
	NewHealth = NewHealth < 0 ? 0 : NewHealth;

	HealthUpdated(NewHealth);

	// Add Health Regen
	GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(RegenDelayTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(RegenDelayTimerHandle, this, &ASPlayer::StartHealthRegen, 1.5f, false, 5.f);
}

void ASPlayer::StartDeath(bool IsDead, AActor* DeadActor)
{
	bIsDead = IsDead;

	GameInstance->SetSpiritsKeepFlag(false);
	GameInstance->SaveEquipment();

	if (IsValid(GetMesh()->GetAnimInstance()))
		GetMesh()->GetAnimInstance()->StopAllMontages(1.0f);

	if (IsValid(GameInstance))
	{
		GameInstance->ClearEquippedItems();
	}

	PlayerController->GetPlayerHUD()->GetLineageCollection()->UpdateLatestLineageEntry();

	CameraFade_BlueprintEvent(2.5f);

	FTimerHandle DeathTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &ASPlayer::LoadSpiritsKeep, 1.0f, false, 2.5f);

	// Assign random ability to inherit, rng based

	const float InheritChance = 0.3f;
	if (FMath::FRand() <= InheritChance)
	{
		TArray<ASAbilityBase*> skillList;
		if (IsValid(GetCurrentSkill()))
			skillList.Add(GetCurrentSkill());
		if (IsValid(GetCurrentSpell()))
			skillList.Add(GetCurrentSpell());
		if (IsValid(GetCurrentPassive()))
			skillList.Add(GetCurrentPassive());

		if (skillList.IsEmpty()) return;

		int32 random = FMath::RandRange(0, skillList.Num() - 1);

		GameInstance->InheritAbility(skillList[random]);

		UE_LOG(LogTemp, Warning, TEXT("Player Death Inheriting Ability"));
	}
}

void ASPlayer::HealthUpdated(const float newHealth)
{
	//This value of newHeath is nomalized from 0 - 1
	FogCleaner->SetColliderRadius((newHealth + 0.2f) * 600.0f);

	if(IsValid(VisualLight))
		LampLight->SetIntensity(LightIntensity * newHealth);

	if (IsValid(PlayerController))
	{
		PlayerController->SetHealthUI(Health, MaxHealth);
	}

	FVector interpolatedPos = FMath::Lerp(EmptyHealthCameraPosition, FullHealthCameraPosition, newHealth);
	MoveCameraToLocalOffset(interpolatedPos);
}

void ASPlayer::RegenerateHealth()
{
	if (Health < MaxHealth && !bIsDead)
	{
		Health = FMath::Clamp(Health + RegenAmount, 0.0f, MaxHealth);

		float NewHealth = (Health / MaxHealth);
		HealthUpdated(NewHealth);

		if (Health >= MaxHealth)
		{
			// Stop regeneration when health is full
			GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
		}
	}
	else
	{
		// Stop regeneration if something unexpected happens
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
	}
}

void ASPlayer::StartHealthRegen()
{
	// Start the health regeneration timer
	if (Health < MaxHealth && !bIsDead)
	{
		GetWorld()->GetTimerManager().SetTimer(HealthRegenTimerHandle, this, &ASPlayer::RegenerateHealth, 1.0f/60.0f, true);
	}
}

#pragma endregion

#pragma region Camera Functions

void ASPlayer::MoveCameraToLocalOffset(const FVector& LocalOffset)
{
	GetWorldTimerManager().ClearTimer(CameraTimerHandle);
	CameraTimerHandle = GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ASPlayer::ProcessCameraMove, LocalOffset));
}

void ASPlayer::ProcessCameraMove(FVector Goal)
{
	FVector currentLocalOffset = MainCamera->GetRelativeLocation();
	if (FVector::Dist(currentLocalOffset, Goal) < 1)
	{
		MainCamera->SetRelativeLocation(Goal);
		return;
	}

	FVector newLocalOffset = FMath::Lerp(currentLocalOffset, Goal, GetWorld()->GetDeltaSeconds() * CameraMoveSpeed);
	MainCamera->SetRelativeLocation(newLocalOffset);
	CameraTimerHandle = GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ASPlayer::ProcessCameraMove, Goal));
}

#pragma endregion

/*
*	We can probably move this functionality to it's own ability component to
*	clean things up
*/
#pragma region Item/Ability Functions

void ASPlayer::EquipItem(USEquipmentData* EquipmentData)
{
	// TODO: Make this also set the mesh for these
	switch (EquipmentData->EquipmentType) 
	{
		case EEquipmentType::WEAPON:
			if (IsValid(WeaponEquipmentData))
				RemoveStats(WeaponEquipmentData->EquipmentStats);

			UE_LOG(LogTemp, Warning, TEXT("Equipping Weapon"));

			WeaponEquipmentData = EquipmentData;
			if (IsValid(GameInstance))
			{
				GameInstance->EquippedItems.EquippedWeapon = WeaponEquipmentData;
			}

			break;
		case EEquipmentType::HEAD:
			if (IsValid(HeadEquipmentData))
				RemoveStats(HeadEquipmentData->EquipmentStats);

			UE_LOG(LogTemp, Warning, TEXT("Equipping Head Gear"));

			HeadEquipmentData = EquipmentData;
			if (IsValid(GameInstance))
			{
				GameInstance->EquippedItems.EquippedHead = HeadEquipmentData;
			}

			break;
		case EEquipmentType::CHEST:
			if (IsValid(ChestEquipmentData))
				RemoveStats(ChestEquipmentData->EquipmentStats);

			UE_LOG(LogTemp, Warning, TEXT("Equipping Chest Gear"));

			ChestEquipmentData = EquipmentData;
			if (IsValid(GameInstance))
			{
				GameInstance->EquippedItems.EquippedChest = ChestEquipmentData;
			}
			break;
		case EEquipmentType::BOOTS:
			if (IsValid(BootEquipmentData))
				RemoveStats(BootEquipmentData->EquipmentStats);

			UE_LOG(LogTemp, Warning, TEXT("Equipping Boots Gear"));

			BootEquipmentData = EquipmentData;
			if (IsValid(GameInstance))
			{
				GameInstance->EquippedItems.EquippedBoot = BootEquipmentData;
				UE_LOG(LogTemp, Warning, TEXT("Inherited Boots Gear"));
			}
			break;
	}

	AddStats(EquipmentData->EquipmentStats);

	WearItem(EquipmentData->EquipmentType, EquipmentData->EquipmentMesh);

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeedCurve->GetFloatValue(Agility);

	if (IsValid(PlayerController))
	{
		PlayerController->SetStatsUI(Strength, Defense, Agility, Soul);
		PlayerController->GetPlayerHUD()->UpdateEquipmentIcon(EquipmentData->EquipmentIcon, EquipmentData->EquipmentType);
	}
}

bool ASPlayer::ObtainItem(ASAbilityBase* newItem)
{
	if (IsValid(newItem) == false) 
	{
		return false;
	}

	USAbilityDataBase* NewAbilityData = newItem->GetAbilityData();
	if (IsValid(NewAbilityData) == false) return false;

	ASAbilityBase* currentItem = GetItemTypeFromNew(newItem);
	USAbilityDataBase* CurrentAbilityData = NewObject<USAbilityDataBase>(this);
	if (IsValid(currentItem))
	{
		CurrentAbilityData = currentItem->GetAbilityData();
	}

	switch (GetItemStatus(newItem, currentItem))
	{
		case EUpgrade::New:
			UE_LOG(LogTemp, Warning, TEXT("New"));

			SetNewAbility(newItem, NewAbilityData);

			PlayerController->AddAbility(NewAbilityData, EUpgrade::New);
			break;

		case EUpgrade::Upgrade:
			if (IsValid(newItem)) {
				newItem->Destroy();
			}

			UE_LOG(LogTemp, Warning, TEXT("Upgrade"));
			if (IsValid(CurrentAbilityData) == false) break;

			PlayerController->AddAbility(CurrentAbilityData, EUpgrade::Upgrade);
			break;

		case EUpgrade::Replace:
			if (IsValid(currentItem)) 
			{
				currentItem->UnRegisterAbility();
			}

			UE_LOG(LogTemp, Warning, TEXT("Replace"));

			SetNewAbility(newItem, NewAbilityData);

			PlayerController->AddAbility(NewAbilityData, EUpgrade::Replace);
			break;
	}

	return false;
}

void ASPlayer::SetNewAbility(ASAbilityBase* newItem, USAbilityDataBase* NewAbilityData)
{
	if (!IsValid(newItem) || !IsValid(NewAbilityData)) return;

	switch (NewAbilityData->GetType())
	{
	case EType::Passive:
		CurrentPassive = newItem;
		if (IsValid(GameInstance))
		{
			GameInstance->SetPassive(newItem);

			PlayerController->GetPlayerHUD()->GetLineageCollection()->UpdateLatestLineageEntry();
			UE_LOG(LogTemp, Warning, TEXT("Adding To Equipped Abilities"));

		}
		break;

	case EType::Skill:
		CurrentSkill = newItem;
		if (IsValid(GameInstance))
		{
			GameInstance->SetSkill(newItem);

			PlayerController->GetPlayerHUD()->GetLineageCollection()->UpdateLatestLineageEntry();
			UE_LOG(LogTemp, Warning, TEXT("Adding To Equipped Abilities"));
		}
		break;

	case EType::Spell:
		CurrentSpell = newItem;
		if (IsValid(GameInstance))
		{
			GameInstance->SetSpell(newItem);

			PlayerController->GetPlayerHUD()->GetLineageCollection()->UpdateLatestLineageEntry();
			UE_LOG(LogTemp, Warning, TEXT("Adding To Equipped Abilities"));
		}
		break;
	}
}

EUpgrade ASPlayer::GetItemStatus(ASAbilityBase* newItem, ASAbilityBase* currentItem)
{
	if (currentItem == nullptr) 
	{
		return EUpgrade::New;
	}
	else if (currentItem->GetAbilityName() == newItem->GetAbilityName())
	{
		return EUpgrade::Upgrade;
	}
	else if (currentItem->GetAbilityName() != newItem->GetAbilityName())
	{
		return EUpgrade::Replace;
	}

	return EUpgrade::New;
}

ASAbilityBase* ASPlayer::GetItemTypeFromNew(ASAbilityBase* newItem)
{
	if (!IsValid(newItem)) 
	{
		return nullptr;
	}
	USAbilityDataBase* NewAbilityData = newItem->GetAbilityData();
	if (IsValid(NewAbilityData) == false)
	{
		return nullptr;
	}

	switch (NewAbilityData->GetType())
	{
		case EType::Passive:
			return CurrentPassive;
			break;

		case EType::Skill:
			return CurrentSkill;
			break;

		case EType::Spell:
			return CurrentSpell;
			break;
	}

	return CurrentPassive;
}

#pragma endregion

#pragma region Helper Functions

FVector ASPlayer::GetMoveFwdDir() const
{
	FVector CamerFwd = MainCamera->GetForwardVector();
	CamerFwd.Z = 0;
	return CamerFwd.GetSafeNormal();
}

FVector ASPlayer::GetMoveRightDir() const
{
	return MainCamera->GetRightVector();
}

void ASPlayer::LoadSpiritsKeep()
{
	if (IsValid(GameInstance))
	{
		GameInstance->UpdateProgress();
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("Spirits_Keep_Intro_Testing"));
}

void ASPlayer::SetLampLightColor(const FLinearColor& NewLampColor)
{
	LampLight->SetLightColor(NewLampColor);
}

void ASPlayer::SetSoulBuff(const ESoulStatType& StatType)
{
	if (StatType == CurrentSoulBuff) return;

	switch (StatType) 
	{
		case ESoulStatType::Strength:
			SetLampLightColor(FLinearColor::Red);
		break;
		case ESoulStatType::Defense:
			SetLampLightColor(FLinearColor::Blue);
		break;
		case ESoulStatType::Agility:
			SetLampLightColor(FLinearColor::Yellow);
		break;
		case ESoulStatType::Soul:
			SetLampLightColor(FLinearColor::Green);
		break;
		default:
			break;
	}

	if (SoulBuff.Contains(StatType)) 
	{
		if(CurrentSoulBuff != ESoulStatType::None)
			RemoveSoulStats(SoulBuff[CurrentSoulBuff]);

		ApplySoulStats(SoulBuff[StatType]);
	}

	if (IsValid(PlayerController))
	{
		PlayerController->SetStatsUI(Strength, Defense, Agility, Soul);
	}

	if(GameInstance)
		GameInstance->CurrentSoulStatType = StatType;

	CurrentSoulBuff = StatType;
}

#pragma endregion

void ASPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
	{
		return;
	}

	if (OtherActor->IsA(ASPlayer::StaticClass()))
	{
		return;
	}


	if (OtherActor->Implements<UIInteractable>() == false)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s Does Not Implement Interactable Interface"), *OtherActor->GetName());

		return;
	}

	InteractableObjects.Add(OtherActor);
	SortInteractableObjects();
}

void ASPlayer::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->Implements<UIInteractable>())
	{
		IIInteractable::Execute_DisableInteractionWidget(OtherActor);
		InteractableObjects.Remove(OtherActor); // Make sure to remove it from the array
		SortInteractableObjects(); // Re-sort and update widgets
	}
}

void ASPlayer::SortInteractableObjects()
{
	InteractableObjects.Sort([this](const AActor& A, const AActor& B)
		{
			return FVector::DistSquared(this->GetActorLocation(), A.GetActorLocation())
				< FVector::DistSquared(this->GetActorLocation(), B.GetActorLocation());
		});

	if (InteractableObjects.Num() == 0) return;

	ASInteractableObject* ClosestObject = Cast<ASInteractableObject>(InteractableObjects[0]);
	if (IsValid(ClosestObject))
	{
		IIInteractable::Execute_EnableInteractionWidget(ClosestObject);
	}

	for (int i = 1; i < InteractableObjects.Num(); i++)
	{
		ASInteractableObject* InteractableObject = Cast<ASInteractableObject>(InteractableObjects[i]);
		if (IsValid(InteractableObject))
		{
			IIInteractable::Execute_DisableInteractionWidget(InteractableObject);
		}
	}
}

AActor* ASPlayer::GetClosestInteractable() const
{
	return InteractableObjects.Num() > 0 ? InteractableObjects[0] : nullptr;
}

void ASPlayer::GetGrabbed()
{
	SetInputMapping(false);
}

void ASPlayer::SetPlayerInputEnabled(bool state)
{
	if (!PlayerController) return;

	UEnhancedInputLocalPlayerSubsystem* inputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!state)
	{
		inputSystem->ClearAllMappings();
	}
	else
	{
		inputSystem->AddMappingContext(InputPlayerMapping, 0);
	}
}

void ASPlayer::CombatEnded() 
{
	bInCombat = false;
	OnCombatEnded.Broadcast();
}

void ASPlayer::OnTutorialSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (ASEnemy* Enemy = Cast<ASEnemy>(OtherActor))
		{
			EnemiesInRange.Add(OtherActor);

			if (EnemiesInRange.Num() == 1 && !bInCombat)
			{
				OnCombatStarted.Broadcast();
				bInCombat = true;
			}
		}

		TutorialComponent->EvaluateObject(OtherActor);
	}
}

void ASPlayer::OnTutorialSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (EnemiesInRange.Contains(OtherActor))
			if (ASEnemy* Enemy = Cast<ASEnemy>(OtherActor))
				EnemiesInRange.Remove(OtherActor);

		if (EnemiesInRange.Num() == 0 && bInCombat)
		{
			if (GetWorld()->GetTimerManager().IsTimerPending(CombatEndedTimer))
			{
				return;
			}

			GetWorld()->GetTimerManager().SetTimer(CombatEndedTimer, this, &ASPlayer::CombatEnded, 1.f / 24.f, false, CombatEndedTimeout);
		}
	}
}

void ASPlayer::DEBUG_ModifyHealth(const FInputActionValue& InputValue)
{
	const float Delta = InputValue.Get<float>();

	Health += (Delta * 5.0f);

	Health = FMath::Clamp(Health, 0, MaxHealth);

	float NewHealth = (Health / MaxHealth);

	NewHealth = FMath::Clamp(NewHealth, 0, 1);

	HealthUpdated(NewHealth);
}


