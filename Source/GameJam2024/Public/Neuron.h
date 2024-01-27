// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chargable.h"
#include "IInteractable.h"
#include "GameFramework/Actor.h"
#include "Neuron.generated.h"

UCLASS()
class GAMEJAM2024_API ANeuron : public AActor , public IChargable, public IInteractable
{

private:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category="Item Data")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere,Category="Item Data")
	USceneComponent* DefaultRoot;

public:	
	
	ANeuron();
	virtual void Interact(AGameJam2024Character* PlayerCharacter, int32 InteractionCode) override;
	virtual void Charge(IChargable* Charger) override;
	virtual void DisCharge() override;
	void BeginFocus();
	void EndFocus();

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
