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

	TArray<AActor*> ActivableArray;

	virtual void ChargeExcange(IChargable* Target)
	{
		if (!CanDischarge || !Target->CanDischarge)
			return;
		if (IsCharged == Target->IsCharged)
			return;

		if (IsCharged)
		{
			Target->Charge();
			Discharge();
		}
		else
		{
			Target->Discharge();
			Charge();
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

	virtual void Charge()
	{
		IsCharged = true;
		for (AActor* ActivableActor : ActivableArray)
		{
			if (ActivableActor->GetClass()->ImplementsInterface(UActivable::StaticClass()))
			{
				Cast<IActivable>(ActivableActor)->Active();
			}
		}
	}

	virtual void Discharge()
	{
		IsCharged = false;
		for (AActor* ActivableActor : ActivableArray)
		{
			if (ActivableActor->GetClass()->ImplementsInterface(UActivable::StaticClass()))
			{
				Cast<IActivable>(ActivableActor)->Deactive();
			}
		}
	}

	virtual void SetCharge(bool newCharge)
	{
		if (newCharge)
			Charge();
		else
			Discharge();
	}
};
