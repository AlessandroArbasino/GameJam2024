// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Activable.h"
#include "IInteractable.h"
#include "GameFramework/Actor.h"
#include "ButtonBase.generated.h"

UCLASS()
class GAMEJAM2024_API AButtonBase : public AActor , public IInteractable
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category="Item Data")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere,Category="Item Data")
	USceneComponent* DefaultRoot;

	
public:	
	
	AButtonBase();

protected:
	virtual void Interact(AGameJam2024Character* PlayerCharacter, int32 InteractionCode) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category="Activable Items")
	TArray<IActivable*> ActivableArray;

	
};
