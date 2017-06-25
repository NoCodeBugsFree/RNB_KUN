// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "RandomActor.h"


// Sets default values
ARandomActor::ARandomActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PickupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	RootComponent = PickupRoot;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->bGenerateOverlapEvents = true;
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ARandomActor::OnPlayerEnterPickupBox);
}

// Called when the game starts or when spawned
void ARandomActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARandomActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	auto NewLocation = GetActorLocation();
	NewLocation.Z += DeltaTime * 100.f;
	SetActorLocation(NewLocation);
}

void ARandomActor::OnPlayerEnterPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Destroy();
}

