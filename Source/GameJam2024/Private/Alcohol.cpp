// Fill out your copyright notice in the Description page of Project Settings.


#include "Alcohol.h"

#include "Components/StaticMeshComponent.h"
#include "GameJam2024/GameJam2024Character.h"


// Sets default values
AAlcohol::AAlcohol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRout=CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	RootComponent=DefaultRout;

	AlcoholCollider=CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	AlcoholCollider->SetupAttachment(DefaultRout);
	     
}

// Called when the game starts or when spawned
void AAlcohol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAlcohol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAlcohol::Charge()
{
	IChargable::Charge();
}

void AAlcohol::Discharge()
{
	IChargable::Discharge();
}

void AAlcohol::ChargeExcange(IChargable* Target)
{
	IChargable::ChargeExcange(Target);
}

