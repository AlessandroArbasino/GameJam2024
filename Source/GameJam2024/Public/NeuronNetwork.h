// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeuronNetwork.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNeuronNetworkDelegate, bool, newCharge);

UCLASS()
class GAMEJAM2024_API ANeuronNetwork : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANeuronNetwork();

	UFUNCTION(BlueprintCallable, Category = "Events")
	void UpdateNeuronNetworkNodes(bool newCharge);

	UPROPERTY()
	FNeuronNetworkDelegate NeuronNetworkUpdate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
