// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SMimic.h"

#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Components/BoxComponent.h"

#include "Engine/World.h"
#include "Framework/SInteractableObject.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Player/SPlayer.h"

#include "Widgets/SItemWidgetComponent.h"

ASMimic::ASMimic()
{
	GrabBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GrabBox"));
	GrabBox->SetupAttachment(GetRootComponent());
	FVector grabBoxRange = FVector(200, 200, 200);
	GrabBox->SetBoxExtent(grabBoxRange);

	ItemWidgetComponent = CreateDefaultSubobject<USItemWidgetComponent>(TEXT("ItemComponent"));
	ItemWidgetComponent->SetupAttachment(GetRootComponent());
}

void ASMimic::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle EscapeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EscapeTimerHandle, this, &ASMimic::Escape, 1.0f, false, EscapeDelay);

	if (IsValid(ItemWidgetComponent->GetWidget()))
	{
		ItemWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}

	OnActorBeginOverlap.AddDynamic(this, &ASMimic::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ASMimic::OnOverlapEnd);

	AIPerceptionSourceComp->Deactivate();
	
	// This is how I think I would Disable it
	AIC = Cast<ASAIController>(GetController());
	if (IsValid(AIC))
	{
		if(IsValid(AIC->GetBlackboardComponent()))
			AIC->GetBlackboardComponent()->Deactivate();
	}

	LastSound = 0.0f;
}

void ASMimic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LastSound++;
	if (LastSound > SoundIncrement)
	{
		MakeSound_BlueprintEvent();
		LastSound = 0.0f;
	}
}

void ASMimic::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("I AM INTERACTING!"));

	AIC = Cast<ASAIController>(GetController());
	if (IsValid(AIC))
	{
		if (IsValid(AIC->GetBlackboardComponent()))
			AIC->GetBlackboardComponent()->Deactivate();
	}

	if (!IsValid(Player)) return;

	Player->OnInteract.RemoveDynamic(this, &ASMimic::Interact);
	OnActorBeginOverlap.RemoveDynamic(this, &ASMimic::OnOverlapBegin);

	if (!IsValid(Player->GetMesh()) || !IsValid(InteractMontage)) return;

	if (!IsValid(Player->GetMesh()->GetAnimInstance())) return;

	UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(InteractMontage, 1.0f);
	}
}

void ASMimic::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	if (!IsValid(Player))
		Player = Cast<ASPlayer>(otherActor);

	if (!IsValid(Player) || Player != otherActor) return;

	if (IsValid(ItemWidgetComponent->GetWidget()))
	{
		ItemWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
	}

	Player->OnInteract.RemoveDynamic(this, &ASMimic::Interact);
	Player->OnInteract.AddDynamic(this, &ASMimic::Interact);
}

void ASMimic::OnOverlapEnd(AActor* overlappedActor, AActor* otherActor)
{
	if (!IsValid(Player)) return;

	if (Player != otherActor) return;

	if (IsValid(ItemWidgetComponent->GetWidget()))
	{
		ItemWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}

	Player->OnInteract.RemoveDynamic(this, &ASMimic::Interact);
}

void ASMimic::StartDeath(bool IsDead)
{
	UE_LOG(LogTemp, Warning, TEXT("I Am Doing this"));

	if (LootPool.IsEmpty() == false) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Loot Table Is Not Empty"));

		const int32 rand = FMath::RandRange(0, LootPool.Num() - 1);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASInteractableObject* Equipment = GetWorld()->SpawnActor<ASInteractableObject>(LootPool[rand], GetActorLocation(), GetActorRotation(), SpawnParams);
	}
	
	Destroy();
}

void ASMimic::Escape()
{
	// Maybe do other things here idk

	Destroy();
}
		