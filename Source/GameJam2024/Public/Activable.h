// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Activable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActivable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEJAM2024_API IActivable
{
	GENERATED_BODY()

public:
	virtual void Active();

	virtual void Deactive();
public:
};
