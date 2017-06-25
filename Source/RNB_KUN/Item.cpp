// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "Engine.h"
#include "Item.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	CollisionBox->bGenerateOverlapEvents = true;
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AItem::TriggerEnter);

	RootComponent = CollisionBox;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RNB_KUNCharacterReference && RNB_KUNCharacterReference->IsInteracting())
	{
		RNB_KUNCharacterReference->Inventory.Add(*ItemName);
		
		SetActorEnableCollision(false);
		RNB_KUNCharacterReference = nullptr;
		Destroy();
	}
}

void AItem::TriggerEnter(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ARNB_KUNCharacter* RNB_KUNCharacter = Cast<ARNB_KUNCharacter>(OtherActor);
	if (RNB_KUNCharacter)
	{
		RNB_KUNCharacterReference = RNB_KUNCharacter;
	}
}
