// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/SPlayerController.h"

#include "Animation/AnimInstance.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"

ASPlayer::ASPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	MainCamera = CreateDefaultSubobject<UCameraComponent>("Main Camera");

	CameraBoom->SetupAttachment(GetRootComponent());
	MainCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	MainCamera->SetWorldRotation(FRotator(0, -65.0f, 0));

	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 500.0;
	CameraBoom->TargetOffset = FVector(0, 0, 1000);

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

	AddMovementInput(input.Y * GetMoveFwdDir() + input.X * GetMoveRightDir());
}

void ASPlayer::Aim(const FInputActionValue& InputValue)
{
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
