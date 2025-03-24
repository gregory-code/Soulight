// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SAbilityBase_FoxFire.h"
#include "Abilities/SAbilityBase.h"

#include "Components/SphereComponent.h"

#include "Enemies/SEnemy.h"

#include "Framework/SCharacterBase.h"
#include "Projectile/SProjectileBase.h"

#include "Kismet/GameplayStatics.h"

ASAbilityBase_FoxFire::ASAbilityBase_FoxFire()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create detection sphere
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->InitSphereRadius(400.0f);
	DetectionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DetectionSphere->SetupAttachment(RootComponent);
}

void ASAbilityBase_FoxFire::BeginPlay()
{
	Super::BeginPlay();

	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASAbilityBase_FoxFire::OnEnemyDetected);
}

void ASAbilityBase_FoxFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Player))
	{
		// Follow player's position
		SetActorLocation(Player->GetActorLocation() + FVector(0.0f, 0.0f, 50.0f));
	}
}

void ASAbilityBase_FoxFire::RegisterAbility(ACharacter* OwningCharacter)
{
	Super::RegisterAbility(OwningCharacter);

	if (!IsValid(OwnerCharacter)) return;

	Player = Cast<ASCharacterBase>(OwnerCharacter);
	if (IsValid(Player))
	{
		Player->OnKill.AddDynamic(this, &ASAbilityBase_FoxFire::OnKill);

		// Attach the ability actor to the player
		AttachToActor(Player, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ASAbilityBase_FoxFire::UnRegisterAbility()
{
	DestroyFoxFireballs();
}

void ASAbilityBase_FoxFire::OnEnemyDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InstancedFoxFireballs.IsEmpty()) return;

	if (!OtherActor) return;

	ASEnemy* Enemy = Cast<ASEnemy>(OtherActor);
	if (!IsValid(Enemy)) return;

	// Notify all fireballs to attack
	for (ASProjectileBase* Fireball : InstancedFoxFireballs)
	{
		if (IsValid(Fireball))
		{
			Fireball->LaunchAtTarget(OtherActor);
		}
	}

	// Clear fireballs after attacking
	InstancedFoxFireballs.Empty();
}

void ASAbilityBase_FoxFire::OnKill()
{
	UE_LOG(LogTemp, Warning, TEXT("Fox Fire Kill Updated"))

	CreateFoxFireball();
}

void ASAbilityBase_FoxFire::CreateFoxFireball()
{
	if (!IsValid(FoxFireballClass) || !IsValid(Player)) return;

	UE_LOG(LogTemp, Warning, TEXT("Creating Fox Fireball"))

	FVector SpawnLocation = Player->GetActorLocation() + FVector(100.0f, 0.0f, 50.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Player;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASProjectileBase* NewFireball = GetWorld()->SpawnActor<ASProjectileBase>(FoxFireballClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (IsValid(NewFireball))
	{
		InstancedFoxFireballs.Add(NewFireball);

		NewFireball->AttachToActor(Player, FAttachmentTransformRules::KeepWorldTransform);
		NewFireball->SetOwner(Player);
		NewFireball->EnableOrbit(Player);
	}
}

void ASAbilityBase_FoxFire::DestroyFoxFireballs()
{
	for (ASProjectileBase* Fireball : InstancedFoxFireballs)
	{
		Fireball->Destroy();
	}
}
