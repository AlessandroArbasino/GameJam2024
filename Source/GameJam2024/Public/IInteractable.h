// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

class AGameJam2024Character;

UENUM()
enum EInteractableType:uint8
{
	Neuron UMETA(DisplayName = "Neuron"),
	Button UMETA(DisplayName = "Button"),
	Terminals UMETA(DisplayName = "Terminals")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditInstanceOnly)
	TEnumAsByte<EInteractableType> InteractableType;

	//ITEMS
	FText Name;
	FText Action;
	int32 Quantity;
	float InteractionDuration;

	//MOVEABLE
	FVector NewWorldPosition;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class GAMEJAM2024_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteraction();
	virtual void EndInteraction();

	//InteractionCode is used for special interaction (like object rotation with R and T keys)
	virtual void Interact(AGameJam2024Character* PlayerCharacter, int32 InteractionCode = 0);
	
	FInteractableData InteractableData;

	int MOVE_GRID_SPACING = 25;
};
