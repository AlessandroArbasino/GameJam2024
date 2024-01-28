// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameJam2024GameMode.h"
#include "GameJam2024Character.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AGameJam2024GameMode::AGameJam2024GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AGameJam2024GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!Player)
		Player=UGameplayStatics::GetPlayerCharacter(this,0);
	else
	{
		if(!IsValid(Player))
			OnDestroy(this);
	}
	
}

void AGameJam2024GameMode::OnDestroy(AActor* DestroyedActor)
{
	UUserWidget* DeadUi = Cast<UUserWidget>(CreateWidget(GetWorld(),WidgeClass));
	if(!DeadUi) return; //spawned just once
	DeadUi->AddToViewport(-1);
	UGameplayStatics::GetPlayerController(this,0)->bShowMouseCursor=true;
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(UGameplayStatics::GetPlayerController(this,0),DeadUi);
}
