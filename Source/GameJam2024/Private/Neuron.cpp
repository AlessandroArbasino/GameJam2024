// Fill out your copyright notice in the Description page of Project Settings.


#include "Neuron.h"

#include "NiagaraComponent.h"
#include "GameJam2024/GameJam2024Character.h"

// Sets default values
ANeuron::ANeuron()
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
void ANeuron::BeginPlay()
{
	Super::BeginPlay();
	ActivableArray = Activables;
	InteractableData.InteractableType = EInteractableType::Neuron;
	if (Network != nullptr)
		Network->NeuronNetworkUpdate.AddDynamic(this, &ANeuron::UpdateNeuronNetwork);

	if (IsChargedOnSpawn)
	{
		IsCharged = true;
		NiagaraComponent->Activate();
	}
}

// Called every frame
void ANeuron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ANeuron::Interact(AGameJam2024Character* PlayerCharacter, int32 InteractionCode)
{
	interactions++;
	if(interactions%2!=0)
	{
		PlayerCharacter->ChargeExcange(this);
	}
	if (IsCharged)
		NiagaraComponent->Activate();
	else
		NiagaraComponent->Deactivate();
	
	if (!Network) return;
	Network->UpdateNeuronNetworkNodes(IsCharged);
}

void ANeuron::BeginFocus()
{
	// UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - BeginFocus"));
	// if (Mesh)
	// {
	// 	Mesh->SetRenderCustomDepth(true);
	// 	UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - SetRenderCustomDepth - true"));
	// }
}

void ANeuron::EndFocus()
{
	// UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - EndFocus"));
	// if (Mesh)
	// {
	// 	Mesh->SetRenderCustomDepth(false);
	// 	UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - SetRenderCustomDepth - false"));
	// }
}

void ANeuron::UpdateNeuronNetwork(bool newCharge)
{
	SetCharge(newCharge);
}
