// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "SniperCharacter.h"
#include "Sniper/Projectile.h"

// Sets default values
ASniperCharacter::ASniperCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	bUseControllerRotationPitch = true;


	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;


	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(GetCapsuleComponent());
	FP_MuzzleLocation->RelativeLocation = FVector();
	FP_MuzzleLocation->RelativeRotation = FRotator();
	
	bIsAiming = false;
}

// Called when the game starts or when spawned
void ASniperCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	/** custom rifle  */
	FP_Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Gun")); //Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->SetWorldScale3D(FVector(3.5f, 3.5f, 3.5f));
}

// Called every frame
void ASniperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASniperCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &ASniperCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASniperCharacter::MoveRight);


	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ASniperCharacter::Fire);

	InputComponent->BindAction("Aim", IE_Pressed, this, &ASniperCharacter::ToggleAiming);
}

void ASniperCharacter::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void ASniperCharacter::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void ASniperCharacter::Fire()
{	
	if (WhatToSpawn)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			
			FVector SpawnLocation = FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * 200.f;
			FRotator SpawnRotation = FirstPersonCameraComponent->GetForwardVector().Rotation();
			FVector End = SpawnLocation + FirstPersonCameraComponent->GetForwardVector() * 10000.f;

			DrawDebugLine(World, SpawnLocation, End, FColor::Red, true, 100.f, 0, 1.f);
			
			AProjectile* Spawned = World->SpawnActor<AProjectile>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WhatToSpawn == NULL"));
	}
}

void ASniperCharacter::ToggleAiming()
{
	bIsAiming = !bIsAiming;

	if (bIsAiming)
	{
		ShowSniperOverlay();
		FirstPersonCameraComponent->SetFieldOfView(10.f);
		FP_Gun->SetVisibility(false);
	} 
	else
	{
		HideSniperOverlay();
		FirstPersonCameraComponent->SetFieldOfView(90.f);
		FP_Gun->SetVisibility(true);
	}
}

