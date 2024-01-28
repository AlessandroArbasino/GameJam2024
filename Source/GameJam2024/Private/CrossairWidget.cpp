// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossairWidget.h"

#include "Components/TextBlock.h"

void UCrossairWidget::SetCrossairColor(float r, float g, float b)
{
	Crossair->SetColorAndOpacity(FSlateColor(FLinearColor(r, g, b, 0.7f)));
}
