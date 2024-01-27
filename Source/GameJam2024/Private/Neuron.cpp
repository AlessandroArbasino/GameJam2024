// Fill out your copyright notice in the Description page of Project Settings.


#include "Neuron.h"

#include "GameJam2024/GameJam2024Character.h"

// Sets default values
ANeuron::ANeuron()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>("Default Scene Root");
	RootComponent =DefaultRoot;

	Mesh= CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(DefaultRoot);

}

// Called when the game starts or when spawned
void ANeuron::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANeuron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ANeuron::Interact(AGameJam2024Character* PlayerCharacter, int32 InteractionCode)
{
	if(PlayerCharacter->GetIsCharged())
	{
		Charge(this);
		PlayerCharacter->DisCharge();
	}
}

void ANeuron::Charge(IChargable* Charger)
{
	IChargable::Charge(Charger);
}

void ANeuron::DisCharge()
{
	IChargable::DisCharge();
}

void ANeuron::BeginFocus()
{
	UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - BeginFocus"));
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
		UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - SetRenderCustomDepth - true"));
	}
}

void ANeuron::EndFocus()
{
	UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - EndFocus"));
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
		UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - SetRenderCustomDepth - false"));
	}
}
