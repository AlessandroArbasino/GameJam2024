// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameJam2024Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "NiagaraComponent.h"
#include "Quaternion.h"
#include "Components/AudioComponent.h"
#include "EntitySystem/MovieSceneComponentDebug.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/Chargable.h"
#include "Public/IInteractable.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGameJam2024Character

AGameJam2024Character::AGameJam2024Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 140.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>("Niagara");
	NiagaraComp->SetAutoActivate(false);
	NiagaraComp->SetupAttachment(RootComponent);

	PropellerAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PropellerAudioComp"));
	PropellerAudioComponent->bAutoActivate = false;
	PropellerAudioComponent->SetupAttachment(RootComponent);

	PropellerAudioComponentCharge = CreateDefaultSubobject<UAudioComponent>(TEXT("PropellerAudioComponentCharge"));
	PropellerAudioComponentCharge->bAutoActivate = false;
	PropellerAudioComponentCharge->SetupAttachment(RootComponent);

	PropellerAudioComponentWhileCharged = CreateDefaultSubobject<UAudioComponent>(
		TEXT("PropellerAudioComponentWhileCharged"));
	PropellerAudioComponentWhileCharged->bAutoActivate = false;
	PropellerAudioComponentWhileCharged->SetupAttachment(RootComponent);

	PropellerAudioComponentBackgroundMusic = CreateDefaultSubobject<UAudioComponent>(
		TEXT("PropellerAudioComponentBackgroundMusic"));
	PropellerAudioComponentBackgroundMusic->bAutoActivate = false;
	PropellerAudioComponentBackgroundMusic->SetupAttachment(RootComponent);
}

void AGameJam2024Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	if (NiagaraComp)
		NiagaraComp->SetActive(false);

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	PropellerAudioComponentBackgroundMusic->SetSound(BackgroundMusic);
	PropellerAudioComponentBackgroundMusic->Play();
}

void AGameJam2024Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsSwing)
	{
		if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
			PerformInteractionCheck();
	}
	if (IsSwing)
		CalculateSwingForce(DeltaSeconds);
}

void AGameJam2024Character::Charge()
{
	IChargable::Charge();
	GetCapsuleComponent()->SetCollisionProfileName("PlayerCharged");
	NiagaraComp->Activate();
	PropellerAudioComponentCharge->SetSound(ChargeTransferSound);
	PropellerAudioComponentCharge->Play();
	PropellerAudioComponentWhileCharged->SetSound(WhileChargedSound);
	PropellerAudioComponentWhileCharged->Play();
}

void AGameJam2024Character::Discharge()
{
	IChargable::Discharge();
	GetCapsuleComponent()->SetCollisionProfileName("Player");
	NiagaraComp->Deactivate();
	PropellerAudioComponentCharge->SetSound(ChargeTransferSound);
	PropellerAudioComponentCharge->Play();
	PropellerAudioComponentWhileCharged->Stop();
}

void AGameJam2024Character::Jump()
{
	Super::Jump();
	PropellerAudioComponent->SetSound(JumpSound);
	PropellerAudioComponent->Play();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGameJam2024Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameJam2024Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameJam2024Character::Look);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this,
		                                   &AGameJam2024Character::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void AGameJam2024Character::Move(const FInputActionValue& Value)
{
	if (IsSwing)
		return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGameJam2024Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//Interactions

//Performed every InteractionCheckFrequency time, not every single frame
void AGameJam2024Character::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	//default values for raycast
	FVector TraceStart = GetPawnViewLocation();
	FVector TraceEnd = TraceStart + GetViewRotation().Vector() * InteractionCheckDistance;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FHitResult HitResult;

	//Collision with object found
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Green, false, 0.2, 0, 5);
			if (HitResult.GetActor() != InteractionData.CurrentInteractable)
			{
				FoundInteractable(HitResult.GetActor());
				return;
			}
			if (HitResult.GetActor() == InteractionData.CurrentInteractable)
				return;
		}
		NoInteractableFound();
	}
	//no collision with objects
	else
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Yellow, false, 0.2, 0, 5);
}

void AGameJam2024Character::FoundInteractable(AActor* NewInteractable)
{
	// if (IsInteracting())
	// 	EndInteract();
	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;
	TargetInteractable->BeginFocus();
}

void AGameJam2024Character::NoInteractableFound()
{
	if (InteractionData.CurrentInteractable)
	{
		//check required for pickup items destroyed in world before triggering the end focus, leading to null reference exception
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AGameJam2024Character::BeginInteract()
{
	//check for changes since interaction begun
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteraction();
			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
		}
	}
}

void AGameJam2024Character::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteraction();
		IsThrowing = false;
	}
}

void AGameJam2024Character::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	if (IsValid(TargetInteractable.GetObject()))
	{
		PropellerAudioComponent->SetSound(PunchSound);
		PropellerAudioComponent->Play();
		TargetInteractable->Interact(this);
		IsThrowing = true;

		if (TargetInteractable->InteractableData.InteractableType == EInteractableType::Button)
		{
			PropellerAudioComponent->SetSound(ButtonPressSound);
			PropellerAudioComponent->Play();
		}
	}
	if (!IsSwing)
	{
		Swing();
	}
	else
	{
		StopSwing();
	}

	if (IsCharged)
		NiagaraComp->Activate();
	else
		NiagaraComp->Deactivate();
}

void AGameJam2024Character::Swing()
{
	if (InteractionData.CurrentInteractable)
	{
		if (TargetInteractable->InteractableData.InteractableType == EInteractableType::Neuron)
		{
			/*SetActorRotation(
				UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
				                                       InteractionData.CurrentInteractable->GetActorLocation()));*/

			PropellerAudioComponent->SetSound(SwingBeginSound);
			PropellerAudioComponent->Play();

			StartingRotator = GetActorRightVector().Rotation();
			StartingRight = GetActorRightVector();
			FVector direction = (GetActorLocation() - InteractionData.CurrentInteractable->GetActorLocation()).
				GetSafeNormal();
			float dot = direction.Dot(FVector::DownVector);
			OffsetAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DownVector.Dot(
				(GetActorLocation() - InteractionData.CurrentInteractable->GetActorLocation()).GetSafeNormal())));
			AttachToActor(InteractionData.CurrentInteractable, FAttachmentTransformRules::KeepWorldTransform);
			IsSwing = true;
			//SwingTimer = 0;
			SwingTimer = UKismetMathLibrary::MapRangeUnclamped(OffsetAngle, 0.f, 60.f, 0.f, 1.f);
			GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
			GetCharacterMovement()->Velocity = FVector::Zero();
		}
	}
}

void AGameJam2024Character::StopSwing()
{
	PropellerAudioComponent->SetSound(SwingReleaseSound);
	PropellerAudioComponent->Play();
	IsSwing = false;
	IsThrowing = false;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FVector Direction = GetActorLocation() - InteractionData.CurrentInteractable->GetActorLocation();
	Direction.Z = JumpForceZ;
	LaunchCharacter((Direction).GetSafeNormal() * JumpForceForward, true, true);
	InteractionData.CurrentInteractable->SetActorRotation(FRotator::ZeroRotator);
	NoInteractableFound();
}

void AGameJam2024Character::CalculateSwingForce(float DeltaTime)
{
	const float LerpRotation = FMath::Sin(SwingTimer);
	const double NewRotation = FMath::Lerp(0, 60, LerpRotation);

	float Distance = FVector::Distance(GetActorLocation(), InteractionData.CurrentInteractable->GetActorLocation());

	if (Distance > MaxSwingDistance)
	{
		// FVector direction = (GetActorLocation() - InteractionData.CurrentInteractable->GetActorLocation()).
		// 	GetSafeNormal();
		// AddActorWorldOffset(direction * -1 * SwingDistanceFixValue);
		//Avvicina player a neurone
	}
	else if (Distance < MinSwingDistance)
	{
		//Allontana player
		// FVector direction = (GetActorLocation() - InteractionData.CurrentInteractable->GetActorLocation()).
		// 	GetSafeNormal();
		// AddActorWorldOffset(direction * SwingDistanceFixValue);
	}

	const FQuat Quat = FQuat(StartingRight, FMath::DegreesToRadians(NewRotation - OffsetAngle));
	InteractionData.CurrentInteractable->SetActorRotation(FRotator::MakeFromEuler(Quat.Euler()));
	SetActorRotation(FRotator(65 * FMath::Sin(65 * FMath::Sin(DeltaTime) / 30), 0, 0));
	SwingTimer += DeltaTime;
}
