// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chargable.h"
#include "IInteractable.h"
#include "NeuronNetwork.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "Neuron.generated.h"

UCLASS()
class GAMEJAM2024_API ANeuron : public AActor, public IChargable, public IInteractable
{
private:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Item Data")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category="Item Data")
	USceneComponent* DefaultRoot;

	UPROPERTY(EditAnywhere, Category="Activable")
	TArray<AActor*> Activables;

	UPROPERTY(EditAnywhere, Category="Network")
	ANeuronNetwork* Network;

	UPROPERTY(EditAnywhere, Category="Item Data")
	bool IsChargedOnSpawn;

	UPROPERTY(VisibleAnywhere, Category="Item Data")
	UNiagaraComponent* NiagaraComponent;

	int32 interactions;

public:
	ANeuron();
	virtual void Interact(AGameJam2024Character* PlayerCharacter, int32 InteractionCode) override;
	void BeginFocus();
	void EndFocus();

	UFUNCTION()
	void UpdateNeuronNetwork(bool newCharge);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
