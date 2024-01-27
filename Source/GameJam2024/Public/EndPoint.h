// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chargable.h"
#include "IInteractable.h"
#include "GameFramework/Actor.h"
#include "EndPoint.generated.h"

UCLASS()
class GAMEJAM2024_API AEndPoint : public AActor, public IInteractable, public IChargable, public IActivable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Item Data")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category="Item Data")
	USceneComponent* DefaultRoot;

public:
	// Sets default values for this actor's properties
	AEndPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Charge() override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(AGameJam2024Character* PlayerCharacter, int32 InteractionCode) override;
	virtual void Active() override;
};
