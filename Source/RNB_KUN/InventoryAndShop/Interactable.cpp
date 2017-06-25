// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "Interactable.h"


// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractableHeplText = FString("Press E to interact with item.");

}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractable::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AInteractable::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("AInteractable::Interact_Implementation() called..."));
}

