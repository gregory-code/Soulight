// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SDreadstorm.h"

#include "Components/WidgetComponent.h"
#include "Components/Widget.h"

#include "Widgets/SHealthbar.h"

ASDreadstorm::ASDreadstorm()
{
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->SetupAttachment(GetRootComponent());
}

void ASDreadstorm::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(WidgetComponent)) return;

	if (!IsValid(WidgetComponent->GetWidget())) return;

	HealthBar = Cast<USHealthbar>(WidgetComponent->GetWidget());
}

void ASDreadstorm::TakeDamage(float Damage)
{
	Super::TakeDamage(Damage);

	if (!IsValid(HealthBar)) return;

	HealthBar->UpdateHealthbar(Health, MaxHealth);
}