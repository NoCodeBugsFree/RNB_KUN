// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "MyActor.h"


// Sets default values
AMyActor::AMyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	
	CollisionComp->bGenerateOverlapEvents = true;
	
	CollisionComp->SetRelativeScale3D(BoxScale);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMyActor::TriggerEnter);
	RootComponent = CollisionComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveThisTick(DeltaTime);

}

void AMyActor::MoveThisTick(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();

	float DeltaHight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));

	if (bMovesYAxis)
	{
		NewLocation.Y += DeltaHight * SpeedScale;
	}
	else
	{
		NewLocation.X += DeltaHight * SpeedScale;
	}

	RunningTime += DeltaTime;
	SetActorLocation(NewLocation);
}

void AMyActor::TriggerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector PlayerStartingLocation = FVector(300, 3, 230);
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		OtherActor->SetActorLocation(PlayerStartingLocation);
	}
}

