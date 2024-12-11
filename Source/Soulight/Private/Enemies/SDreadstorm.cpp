// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SDreadstorm.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Widget.h"

#include "Widgets/SHealthbar.h"

ASDreadstorm::ASDreadstorm()
{
	HeadHurtbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Head Hurtbox"));
	HeadHurtbox->SetupAttachment(GetRootComponent());

	LeftHandHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Hand Hitbox"));
	LeftHandHitbox->SetupAttachment(GetRootComponent());

	RightHandHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Hand Hitbox"));
	RightHandHitbox->SetupAttachment(GetRootComponent());

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->SetupAttachment(GetRootComponent());
}

void ASDreadstorm::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(WidgetComponent)) return;

	if (!IsValid(WidgetComponent->GetWidget())) return;

	if (!IsValid(WidgetComponent->GetWidgetClass())) return;

	//HealthBar = Cast<USHealthbar>(WidgetComponent->GetWidget());

	HealthBar = CreateWidget<USHealthbar>(GetWorld(), WidgetComponent->GetWidgetClass());

	if (IsValid(GetMesh()))
	{
		if (GetMesh()->DoesSocketExist(LeftHandSocketName))
		{
			LeftHandHitbox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandSocketName);
		}

		if (GetMesh()->DoesSocketExist(LeftHandSocketName))
		{
			RightHandHitbox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandSocketName);
		}
	}

	if (!IsValid(HealthBar)) return;
	WidgetComponent->SetWidget(HealthBar);

	HealthBar->UpdateHealthbar(Health, MaxHealth);
}

void ASDreadstorm::TakeDamage(float Damage, AActor* DamageInstigator, const float& Knockback)
{
	Super::TakeDamage(Damage, DamageInstigator, 0);

	if (!IsValid(HealthBar)) return;

	HealthBar->UpdateHealthbar(Health, MaxHealth);
}

void ASDreadstorm::RaiseHead()
{
	HeadHurtbox->SetActive(false);

	bHeadRaised = true;
}

void ASDreadstorm::LowerHead()
{
	HeadHurtbox->SetActive(true);

	bHeadRaised = false;
}
