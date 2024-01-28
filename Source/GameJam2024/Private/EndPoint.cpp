// Fill out your copyright notice in the Description page of Project Settings.


#include "EndPoint.h"

#include "GameJam2024/GameJam2024Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEndPoint::AEndPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>("Default Scene Root");
	RootComponent = DefaultRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(DefaultRoot);
}

// Called when the game starts or when spawned
void AEndPoint::BeginPlay()
{
	Super::BeginPlay();
	InteractableData.InteractableType = EInteractableType::Terminals;
	IsCharged = false;
	ActivableArray.Add(this);
}

// Called every frame
void AEndPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEndPoint::Charge()
{
	IChargable::Charge();
	Active();
}

void AEndPoint::BeginFocus()
{
	IInteractable::BeginFocus();
}

void AEndPoint::EndFocus()
{
	IInteractable::EndFocus();
}

void AEndPoint::Interact(AGameJam2024Character* PlayerCharacter, int32 InteractionCode)
{
	IInteractable::Interact(PlayerCharacter, InteractionCode);
	PlayerCharacter->ChargeExcange(this);
}

void AEndPoint::Active()
{
	IActivable::Active();
	//TODO END LEVEL AND CHANGE LEVEL
	UGameplayStatics::OpenLevel(GetWorld(),"CinematicViewLevelEnd");
}
