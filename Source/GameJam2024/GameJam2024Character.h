// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Chargable.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GameJam2024Character.generated.h"

class IInteractable;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData()
	{
		CurrentInteractable = nullptr;
		LastInteractionCheckTime = 0.0f;
	}

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS(config=Game)
class AGameJam2024Character : public ACharacter,public IChargable
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IInteractable", meta = (AllowPrivateAccess = "true"))
	TScriptInterface<IInteractable> TargetInteractable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	float InteractionCheckFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;
	FInteractionData InteractionData;

public:
	AGameJam2024Character();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();
	void Tick(float DeltaSeconds);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	virtual void Charge(IChargable* Charger) override;
	virtual void DisCharge() override;
};

