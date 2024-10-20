// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/SPlayerController.h"
#include "InputCoreTypes.h"
#include "GameFramework/PlayerInput.h"

#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

#include "Abilities/SAbilityBase.h"
#include "Animation/AnimInstance.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Framework/SFogCleaner.h"

#include "Player/Abilities/SAbilityDataBase.h"

#include "Components/StaticMeshComponent.h"

#include "Components/SceneCaptureComponent2D.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"

ASPlayer::ASPlayer()
{
	MainCameraPivot = CreateDefaultSubobject<USceneComponent>("Pivot of Main Camera");
	FullHealthView = CreateDefaultSubobject<USceneComponent>("Full Health Pos");
	EmptyHealthView = CreateDefaultSubobject<USceneComponent>("Empty Health Pos");
	MainCamera = CreateDefaultSubobject<UCameraComponent>("Main Camera");

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

	if (FogCleaner)
	{
		FogCleaner->SetActorRelativeLocation(GetActorLocation());
	}
}

void ASPlayer::BeginPlay()
{
	Super::BeginPlay();

	HUDInputAction->bTriggerWhenPaused = true;
	SettingsInputAction->bTriggerWhenPaused = true;

	FInputModeGameOnly input;
	GetWorld()->GetFirstPlayerController()->SetInputMode(input);

	FActorSpawnParameters spawnParam;
	FVector spawnPos = GetActorLocation();

	FogCleaner = GetWorld()->SpawnActor<ASFogCleaner>(mFogCleanerClass, spawnPos, FRotator(0, 0, 0), spawnParam);
}

void ASPlayer::TakeDamage(float Damage)
{
	Super::TakeDamage(Damage);

	EndCombo();

	float NewHealth = (Health / MaxHealth);
	NewHealth = NewHealth < 0 ? 0 : NewHealth;

	HealthUpdated(NewHealth);
}

void ASPlayer::PawnClientRestart()
{
	Super::PawnClientRestart();

	PlayerController = GetController<ASPlayerController>();
	SetInputMapping(true);
}

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
	}
}

void ASPlayer::Move(const FInputActionValue& InputValue)
{
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
	FVector2D input = InputValue.Get<FVector2D>();
	input.Normalize();

	GetCharacterMovement()->bOrientRotationToMovement = false;

	previousDir = FVector(input.X, input.Y, 0.0f);

	SetActorRotation(FVector(input.X, input.Y, 0.0f).ToOrientationRotator());
}

void ASPlayer::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacted"));

	OnInteract.Broadcast(true);
}

void ASPlayer::Attack()
{
	if (bCanAttack == false) return;

	//if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) return;

	AttackCombo();
}

void ASPlayer::Dodge()
{
	HealthUpdated(0.5f);
}

void ASPlayer::Skill()
{
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
	if (PlayerController)
	{
		bHUDEnabled = !bHUDEnabled;

		PlayerController->GameplayUIState(bHUDEnabled);
	}
}

void ASPlayer::Settings()
{

}

void ASPlayer::AttackCombo()
{
	if (!IsValid(AttackComboMontage)) return;

	bCanAttack = false;
	bHasAttacked = true;

	UE_LOG(LogTemp, Warning, TEXT("Section: %d"), CurrentCombo);

	if (!IsValid(GetMesh()->GetAnimInstance())) return;

	//GetMesh()->GetAnimInstance()->StopAllMontages(1.0f);

	GetMesh()->GetAnimInstance()->Montage_Play(ComboSectionMontages[CurrentCombo]);
}

void ASPlayer::EndCombo()
{
	bHasAttacked = false;
	bCanAttack = true;
	CurrentCombo = 0;
}

void ASPlayer::GetGrabbed()
{
	SetInputMapping(false);
}

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

void ASPlayer::HealthUpdated(const float newHealth)
{
	//This value of newHeath is nomalized from 0 - 1
	FogCleaner->SetColliderRadius((newHealth + 0.2f) * 600.0f);

	FVector interpolatedPos = FMath::Lerp(EmptyHealthView->GetRelativeLocation(), FullHealthView->GetRelativeLocation(), newHealth);
	MoveCameraToLocalOffset(interpolatedPos);
}

bool ASPlayer::ObtainItem(ASAbilityBase* newItem)
{
	if (IsValid(newItem) == false) 
	{
		UE_LOG(LogTemp, Warning, TEXT("New Item Is Null"));
		return false;
	}

	USAbilityDataBase* NewAbilityData = newItem->GetAbilityData();
	if (IsValid(NewAbilityData) == false) return false;

	ASAbilityBase* currentItem = GetItemTypeFromNew(newItem);
	USAbilityDataBase* CurrentAbilityData = NewObject<USAbilityDataBase>(this);
	if (IsValid(currentItem))
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating Current Ability Data"));

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
				currentItem->Destroy();
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
		break;

	case EType::Skill:
		CurrentSkill = newItem;
		break;

	case EType::Spell:
		CurrentSpell = newItem;
		break;
	}
}


EUpgrade ASPlayer::GetItemStatus(ASAbilityBase* newItem, ASAbilityBase* currentItem)
{
	UE_LOG(LogTemp, Warning, TEXT("Part 2"));

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
	UE_LOG(LogTemp, Warning, TEXT("I am going into this function"));

	if (!IsValid(newItem)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Is Null"));

		return nullptr;
	}
	USAbilityDataBase* NewAbilityData = newItem->GetAbilityData();
	if (IsValid(NewAbilityData) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("New Ability Data is null"));

		return nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("Part 202"));

	switch (NewAbilityData->GetType())
	{
		case EType::Passive:
			UE_LOG(LogTemp, Warning, TEXT("Passive"));

			return CurrentPassive;
			break;

		case EType::Skill:
			UE_LOG(LogTemp, Warning, TEXT("Skill"));

			return CurrentSkill;
			break;

		case EType::Spell:
			UE_LOG(LogTemp, Warning, TEXT("Spell"));

			return CurrentSpell;
			break;
	}
	UE_LOG(LogTemp, Warning, TEXT("Part 303"));

	return CurrentPassive;
}