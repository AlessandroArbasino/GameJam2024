// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameJam2024GameMode.generated.h"

UCLASS(minimalapi)
class AGameJam2024GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameJam2024GameMode();

	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
	void OnDestroy(AActor* DestroyedActor);

	UPROPERTY()
	TObjectPtr<ACharacter> Player;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WidgeClass;
};



