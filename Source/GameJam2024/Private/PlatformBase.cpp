// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformBase.h"

#include "Components/SplineComponent.h"

// Sets default values
APlatformBase::APlatformBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultRout = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent=DefaultRout;

	PlatformRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PlatformRoot"));
	PlatformRoot->SetupAttachment(DefaultRout);
	
	Mesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(PlatformRoot);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(DefaultRout);
	
}

// Called when the game starts or when spawned
void APlatformBase::BeginPlay()
{
	Super::BeginPlay();
	if(PlatformType == EPlatformType::Toggle)
	{
		SetActorStatus(IsActive,this);
	}
	
}

// Called every frame
void APlatformBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsMoving)
	{
		if(IsLooping || Timer<=PI)
		{
			PlatformRoot->SetRelativeLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength()*
				((FMath::Sin(Timer*PlatformSpeed-PI/2)+1)/2),ESplineCoordinateSpace::Local));
		}
		Timer+=DeltaTime;
	}
}

void APlatformBase::Active()
{
	IActivable::Active();
	switch (PlatformType)
	{
	case EPlatformType::Moving:
		IsMoving=true;
		break;
	case EPlatformType::Toggle:
		IsMoving=true;
		IsActive=!IsActive;
		SetActorStatus(IsActive,this);
		break;
	}
}

void APlatformBase::Deactive()
{
	IActivable::Deactive();
	
	switch (PlatformType)
	{
	case EPlatformType::Moving:
		IsMoving=false;
		break;
	case EPlatformType::Toggle:
		IsMoving=false;
		IsActive=!IsActive;
		SetActorStatus(IsActive,this);
		break;
	}
}
void APlatformBase::SetActorStatus(bool ActiveStatus, AActor* ActorReference)
{
	ActorReference->SetActorHiddenInGame(!ActiveStatus);
	ActorReference->SetActorEnableCollision(ActiveStatus);
	ActorReference->SetActorTickEnabled(ActiveStatus);
}

