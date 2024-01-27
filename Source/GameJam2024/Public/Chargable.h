// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Activable.h"
#include "UObject/Interface.h"
#include "Chargable.generated.h"

UINTERFACE(MinimalAPI)
class UChargable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEJAM2024_API IChargable
{
	GENERATED_BODY()

public :
	bool CanDischarge = true;
	
	bool IsCharged = false;

	TArray<IActivable*> ActivableArray;

	virtual void Charge(IChargable* Charger)
	{
		IsCharged = true;
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, "Object Charged");
		for (IActivable* Activable : ActivableArray)
		{
			Activable->Active();
		}
		Charger->DisCharge();
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, "Charged Discharged");
	}

	virtual void DisCharge()
	{
		if (CanDischarge)
		{
			IsCharged = false;

			for (IActivable* Activable : ActivableArray)
			{
				Activable->Deactive();
			}
		}
	}

	bool GetIsCharged() const
	{
		return IsCharged;
	}

	bool GetCanDischarge() const
	{
		return CanDischarge;
	}
};
