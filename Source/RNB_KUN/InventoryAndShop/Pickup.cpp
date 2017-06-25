// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "Pickup.h"
#include "RNB_KUNCharacter.h"

APickup::APickup()
{
	// set all values
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interactable Mesh"));
	RootComponent = InteractableMesh;

	InteractableMesh->SetSimulatePhysics(true);
	
	InteractableHeplText = FString("Press E to pick item up");

	PickupThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("Pickup Thumbnail"));
}

void APickup::BeginPlay()
{
	InteractableHeplText = FString::Printf(TEXT("Press E to pick up a %s"), *ItemName);
}

void APickup::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("APickup::Interact_Implementation() called..."));
	ARNB_KUNCharacter* RNB_KUNCharacter = Cast<ARNB_KUNCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));

	// places the item into the character's inventory
	if (RNB_KUNCharacter->AddItemToInventory(this))
	{
		OnPickedup();
	}
}

void APickup::OnPickedup()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APickup::Use_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("APickup::Use_Implementation() called..."));
}
