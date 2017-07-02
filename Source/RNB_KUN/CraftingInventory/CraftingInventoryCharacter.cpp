// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "CraftingInventoryCharacter.h"
#include "CraftingInventory/CrafringPlayerController.h"
#include "CraftingInventory/CraftingPickup.h"
#include "CraftingInventory/InteractableActor.h"

ACraftingInventoryCharacter::ACraftingInventoryCharacter()
{	
	FPS_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	FPS_Camera->SetupAttachment(GetCapsuleComponent());
	FPS_Camera->bUsePawnControlRotation = true;
	FPS_Camera->SetRelativeLocation(FVector(20.f, 0.f, 70.f));
}

void ACraftingInventoryCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACraftingInventoryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();
}

void ACraftingInventoryCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &ACraftingInventoryCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACraftingInventoryCharacter::MoveRight);

	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

}

void ACraftingInventoryCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACraftingInventoryCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACraftingInventoryCharacter::CheckForInteractables()
{
 	FVector Direction = GetFPS_Camera()->GetForwardVector();
 	FVector Start = GetFPS_Camera()->GetComponentLocation();
 	FVector End = Start + (Direction * Distance);
 	
 	FHitResult HitResult;
 	FCollisionQueryParams CollisionQueryParams;
 	CollisionQueryParams.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionQueryParams);

	ACrafringPlayerController* CrafringPlayerController = Cast<ACrafringPlayerController>(GetController());

	if (HitResult.GetActor())
	{
		if (AInteractableActor* InteractableActor = Cast<AInteractableActor>(HitResult.GetActor()))
		{
			if (CrafringPlayerController)
			{
				CrafringPlayerController->SetCurrentInteractable(InteractableActor);
				return;
			}
		}
	}

	CrafringPlayerController->SetCurrentInteractable(nullptr);

 	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 0.1f, (uint8)0, 2.f);
}
