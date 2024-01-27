// Fill out your copyright notice in the Description page of Project Settings.


#include "Neuron.h"

#include "GameJam2024/GameJam2024Character.h"

// Sets default values
ANeuron::ANeuron()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
