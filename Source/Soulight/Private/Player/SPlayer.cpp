// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/SPlayerController.h"
#include "InputCoreTypes.h"
#include "GameFramework/PlayerInput.h"

#include "Animation/AnimInstance.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"

ASPlayer::ASPlayer()
{
	FullHealthView = CreateDefaultSubobject<USceneComponent>("Full Health Pos");
	EmptyHealthView = CreateDefaultSubobject<USceneComponent>("Empty Health Pos");
	MainCamera = CreateDefaultSubobject<UCameraComponent>("Main Camera");

	FullHealthView->SetupAttachment(GetRootComponent());

	EmptyHealthView->SetupAttachment(GetRootComponent());
	MainCamera->SetupAttachment(GetRootComponent());
	MainCamera->SetWorldRotation(FRotator(0, -60.0f, 0));

	//CameraBoom->bUsePawnControlRotation = true;
	//CameraBoom->TargetArmLength = 600.0f;
	//CameraBoom->TargetOffset = FVector(0, 0, 1000);

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(1080.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
}

void ASPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void ASPlayer::PawnClientRestart()
{
	Super::PawnClientRestart();

	PlayerController = GetController<ASPlayerController>();
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* inputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		inputSystem->ClearAllMappings();
		inputSystem->AddMappingContext(InputMapping, 0);
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
		enhancedInputComponent->BindAction(AttackInputAction, ETriggerEvent::Triggered, this, &ASPlayer::Attack);
		enhancedInputComponent->BindAction(DodgeInputAction, ETriggerEvent::Triggered, this, &ASPlayer::Dodge);
		enhancedInputComponent->BindAction(SkillInputAction, ETriggerEvent::Triggered, this, &ASPlayer::Skill);
		enhancedInputComponent->BindAction(SpellInputAction, ETriggerEvent::Triggered, this, &ASPlayer::Spell);
		enhancedInputComponent->BindAction(HUDInputAction, ETriggerEvent::Triggered, this, &ASPlayer::HUD);
		enhancedInputComponent->BindAction(SettingsInputAction, ETriggerEvent::Triggered, this, &ASPlayer::Settings);
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
}

void ASPlayer::Attack()
{
}

void ASPlayer::Dodge()
{
}

void ASPlayer::Skill()
{
}

void ASPlayer::Spell()
{
}

void ASPlayer::HUD()
{
}

void ASPlayer::Settings()
{
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
