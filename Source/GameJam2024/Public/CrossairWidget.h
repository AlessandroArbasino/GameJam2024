// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CrossairWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM2024_API UCrossairWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Crossair;

public:
	UFUNCTION()
	void SetCrossairColor(float r, float g, float b);
};
