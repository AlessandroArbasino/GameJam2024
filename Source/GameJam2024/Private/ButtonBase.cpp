// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonBase.h"


// Sets default values
AButtonBase::AButtonBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>("Default Scene Root");
	RootComponent =DefaultRoot;

	Mesh= CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(DefaultRoot);

	
}



// Called when the game starts or when spawned
void AButtonBase::BeginPlay()
{
	Super::BeginPlay();

	InteractableData.InteractableType = EInteractableType::Button;
}

// Called every frame
void AButtonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AButtonBase::Interact(AGameJam2024Character* PlayerCharacter, int32 InteractionCode)
{
	for (AActor*  Activable : ActivableActor)
	{
		if(Activable->GetClass()->ImplementsInterface(UActivable::StaticClass()))
			Cast<IActivable>(Activable)->Active();
		UE_LOG(LogTemp,Warning, TEXT("{0}"))
	}
}
void AButtonBase::BeginFocus()
{
	// UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - BeginFocus"));
	// if (Mesh)
	// {
	// 	Mesh->SetRenderCustomDepth(true);
	// 	UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - SetRenderCustomDepth - true"));
	// }
}

void AButtonBase::EndFocus()
{
	// UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - EndFocus"));
	// if (Mesh)
	// {
	// 	Mesh->SetRenderCustomDepth(false);
	// 	UE_LOG(LogTemp, Warning, TEXT("AGenericMovable - SetRenderCustomDepth - false"));
	// }
}

