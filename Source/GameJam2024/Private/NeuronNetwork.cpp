// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuronNetwork.h"

// Sets default values
ANeuronNetwork::ANeuronNetwork()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

//FMyCustomDelegate ANeuronNetwork::OnCustomEvent;

void ANeuronNetwork::UpdateNeuronNetworkNodes(bool newCharge)
{
	NeuronNetworkUpdate.Broadcast(newCharge);
}	

// Called when the game starts or when spawned
void ANeuronNetwork::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANeuronNetwork::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
