// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chargable.h"
#include "GameFramework/Actor.h"
#include "Alcohol.generated.h"


class UStaticMeshComponent;

UCLASS()
class GAMEJAM2024_API AAlcohol : public AActor, public IChargable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* AlcoholCollider;

	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultRout;
	
public:

	// Sets default values for this actor's properties
	AAlcohol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Charge() override;
	virtual void Discharge() override;
	virtual void ChargeExcange(IChargable* Target) override;
};
