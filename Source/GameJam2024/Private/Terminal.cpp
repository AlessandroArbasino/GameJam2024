// Fill out your copyright notice in the Description page of Project Settings.


#include "Terminal.h"

#include "GameJam2024/GameJam2024Character.h"

// Sets default values
ATerminal::ATerminal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>("Default Scene Root");
	RootComponent = DefaultRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(DefaultRoot);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraVFX");
	NiagaraComponent->SetupAttachment(DefaultRoot);
	NiagaraComponent->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ATerminal::BeginPlay()
{
	Super::BeginPlay();
	InteractableData.InteractableType = EInteractableType::Terminals;

	if (IsChargedOnSpawn)
	{
		IsCharged = true;
		NiagaraComponent->Activate();
	}
	ActivableArray = Activables;
}

// Called every frame
void ATerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATerminal::BeginFocus()
{
	// UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - BeginFocus"));
	// if (Mesh)
	// {
	// 	Mesh->SetRenderCustomDepth(true);
	// 	UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - SetRenderCustomDepth - true"));
	// }
}

void ATerminal::EndFocus()
{
	// UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - EndFocus"));
	// if (Mesh)
	// {
	// 	Mesh->SetRenderCustomDepth(false);
	// 	UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - SetRenderCustomDepth - false"));
	// }
}

void ATerminal::Interact(AGameJam2024Character* PlayerCharacter, int32 InteractionCode)
{
	PlayerCharacter->ChargeExcange(this);
	if (IsCharged)
		NiagaraComponent->Activate();
	else
		NiagaraComponent->Deactivate();
}
