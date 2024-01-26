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

private:
	bool CanDischarge=false;
	bool IsCharged=false;

	TArray<IActivable> ActivableArray;
public :

	void Charge(IChargable Charger)
	{
		IsCharged=true;
	
		for (auto Activable : ActivableArray)
		{
			Activable.Active();
		}
	
		Charger.DisCharge();
	}

	void DisCharge()
	{
		if(CanDischarge)
		{
			IsCharged=false;
			
			for (auto Activable : ActivableArray)
			{
				Activable.Deactive();
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

public:
};
