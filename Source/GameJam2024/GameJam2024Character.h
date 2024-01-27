// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Logging/LogMacros.h"
#include "Public/Chargable.h"
#include "Public/IInteractable.h"
#include "CableComponent.h"
#include "GameJam2024Character.generated.h"
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
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
class AGameJam2024Character : public ACharacter, public IChargable
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IInteractable", meta = (AllowPrivateAccess = "true"))
	TScriptInterface<IInteractable> TargetInteractable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	float InteractionCheckFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;
	FInteractionData InteractionData;

	UPROPERTY()
	USceneComponent* SwingPivot;

	UPROPERTY()
	float SwingTimer = 0;

	UPROPERTY()
	FRotator StartingRotator;

public:
	AGameJam2024Character();

	UPROPERTY(VisibleAnywhere, Category="Animation Control")
	bool IsSwing;

	UPROPERTY(VisibleAnywhere)
	float BoneScale = 1.0f;

	UPROPERTY(VisibleAnywhere)
	float CableLenghtPlayer = 1.0f;

	UPROPERTY(VisibleAnywhere)
	FVector LaunchSpeed{0, 0, 100};

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

	void Swing();
	void StopSwing();
	void CalculateSwingForce(float DeltaTime);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	virtual void Charge() override;
	virtual void Discharge() override;
};
