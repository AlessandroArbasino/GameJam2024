// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Chargable.generated.h"

// This class does not need to be modified.
DECLARE_MULTICAST_DELEGATE(OnCharge)
DECLARE_MULTICAST_DELEGATE(OnDischarge)
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

	OnDischarge DischargeEvent;
	OnCharge ChargeEvent;
	
public :

	void Charge(IChargable Charger)
	{
		IsCharged=true;
		ChargeEvent.Broadcast();
		Charger.DisCharge();
	}

	void DisCharge()
	{
		if(CanDischarge)
		{
			IsCharged=false;
			DischargeEvent.Broadcast();
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
