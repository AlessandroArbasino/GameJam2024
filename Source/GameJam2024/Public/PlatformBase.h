// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Activable.h"
#include "GameFramework/Actor.h"
#include "PlatformBase.generated.h"


class USplineComponent;

UENUM(BlueprintType)
enum class EPlatformType: uint8 {Moving,Toggle};

UCLASS()
class GAMEJAM2024_API APlatformBase : public AActor,public IActivable
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultRout;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* PlatformRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere,Category="Platform Params")
	EPlatformType PlatformType;
	
	UPROPERTY(EditAnywhere,Category="Platform Params")
	float PlatformSpeed=1;

	UPROPERTY(EditAnywhere,Category="Platform Params")
	bool IsLooping = true;

	UPROPERTY()
	bool IsMoving = false;
	
	UPROPERTY()
	float Timer=0;

public:	
	// Sets default values for this actor's properties
	APlatformBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Active() override;
	
	UFUNCTION()
	virtual void Deactive() override;

	UFUNCTION()
	void SetActorStatus(bool ActiveStatus, AActor* ActorReference);
};
