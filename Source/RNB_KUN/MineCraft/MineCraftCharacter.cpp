// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "MineCraftCharacter.h"
#include "Block.h"
#include "Wieldable.h"

// Sets default values
AMineCraftCharacter::AMineCraftCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	FP_WeildedItem = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PickAxeMesh"));
	FP_WeildedItem->SetOnlyOwnerSee(true);
	FP_WeildedItem->SetupAttachment(Mesh1P, FName("Gun"));
	FP_WeildedItem->bCastDynamicShadow = false;
	FP_WeildedItem->CastShadow = false;

	bIsBrealing = false;
}

// Called when the game starts or when spawned
void AMineCraftCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	/** Initialize inventory  */
	Inventory.SetNum(NUM_OF_INVENTORY_SLOTS, false);
}

// Called every frame
void AMineCraftCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckForBlocks();
}

void AMineCraftCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMineCraftCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMineCraftCharacter::CheckForBlocks()
{
	FVector Direction = FirstPersonCameraComponent->GetForwardVector();
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Start + (Direction * Reach);
	
	FHitResult OutHit;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionQueryParams);
	
	ABlock* PotentialBlock = Cast<ABlock>(OutHit.GetActor());

	/** Protect from starting damage the block and turn another direction - in this situation we intended to stop breaking the block  */
	if (PotentialBlock != CurrentBlock && CurrentBlock)
	{
		CurrentBlock->ResetBlock();
	}

	if (PotentialBlock == nullptr)
	{
		CurrentBlock = nullptr;
		return;
	}
	else
	{
		CurrentBlock = PotentialBlock;
	}
}

AWieldable* AMineCraftCharacter::GetCurrentWieldableItem() const
{
	if (Inventory.Num() > 0)
	{
		return Inventory[CurrentInventorySlot];
	}
	return nullptr;
}

void AMineCraftCharacter::Throw()
{
	AWieldable* ItemToThrow = GetCurrentWieldableItem();

	FHitResult LineTraceHit;
	
	FVector Direction = GetActorForwardVector();
	FVector Start = FirstPersonCameraComponent->GetComponentLocation(); 
	FVector End = Start + (Direction * Reach);
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByChannel(LineTraceHit, Start, End, ECC_WorldDynamic, CollisionQueryParams);
	
	FVector DropLocation = End;

	if (LineTraceHit.GetActor())
	{
		DropLocation = LineTraceHit.ImpactPoint - (Direction * FMath::RandRange(10.f, 30.f));
	}

	if (ItemToThrow)
	{
		ItemToThrow->SetActorLocationAndRotation(DropLocation, FRotator::ZeroRotator);
		ItemToThrow->Hide(false);
		Inventory[CurrentInventorySlot] = nullptr;
	}

	UpdateWieldedItem();
}

void AMineCraftCharacter::UpdateWieldedItem()
{
	if (Inventory.Num() > 0 )
	{
		/**  show SkeletalMesh */
		if (Inventory[CurrentInventorySlot])
		{	
			USkeletalMesh* NewSKMesh = Inventory[CurrentInventorySlot]->GetWieldableMesh()->SkeletalMesh;
			FP_WeildedItem->SetSkeletalMesh(NewSKMesh);
		} 
		else /** hide SkeletalMesh  */
		{
			FP_WeildedItem->SetSkeletalMesh(nullptr);
		}
	}
}

void AMineCraftCharacter::MoveUpInventorySlot()
{
	CurrentInventorySlot = FMath::Abs((CurrentInventorySlot + 1) % NUM_OF_INVENTORY_SLOTS);
	UpdateWieldedItem();
}

void AMineCraftCharacter::MoveDownInventorySlot()
{
	if (CurrentInventorySlot == 0)
	{
		CurrentInventorySlot = 9;
		UpdateWieldedItem();
		return;
	}
	CurrentInventorySlot = FMath::Abs((CurrentInventorySlot - 1) % NUM_OF_INVENTORY_SLOTS);
	UpdateWieldedItem();
}

void AMineCraftCharacter::OnHit()
{
	/** Only Hit if we have a Wieldable Item equipped   */
	if (GetCurrentWieldableItem())
	{
		PlayHitAnimation();
		if (CurrentBlock)
		{
			bIsBrealing = true;
			float TimeBetweenBreaks = CurrentBlock->GetResistance() / 100.f / 2; //  2 is CurrentTool->GetPower();
			GetWorldTimerManager().SetTimer(BlockBreakingTimer, this, &AMineCraftCharacter::BreakBlock, TimeBetweenBreaks, true);
			GetWorldTimerManager().SetTimer(HitAnimationTimer, this, &AMineCraftCharacter::PlayHitAnimation, 0.4f, true);
		}
	}
}

void AMineCraftCharacter::EndHit()
{
	bIsBrealing = false;
	GetWorldTimerManager().ClearAllTimersForObject(this);
	if (CurrentBlock)
	{
		CurrentBlock->ResetBlock();
	}
}

void AMineCraftCharacter::PlayHitAnimation()
{
	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AMineCraftCharacter::BreakBlock()
{
	if (bIsBrealing && CurrentBlock && !CurrentBlock->IsPendingKill())
	{
		CurrentBlock->Break();
	}
}

// Called to bind functionality to input
void AMineCraftCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMineCraftCharacter::OnHit);
	InputComponent->BindAction("Fire", IE_Released, this, &AMineCraftCharacter::EndHit);

	InputComponent->BindAction("Throw", IE_Pressed, this, &AMineCraftCharacter::Throw);
	
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	InputComponent->BindAxis("MoveForward", this, &AMineCraftCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMineCraftCharacter::MoveRight);

	InputComponent->BindAction("InventoryUp", IE_Pressed, this, &AMineCraftCharacter::MoveUpInventorySlot);
	InputComponent->BindAction("InventoryDown", IE_Pressed, this, &AMineCraftCharacter::MoveDownInventorySlot);
	
}

void AMineCraftCharacter::SetMaterialType(EMaterial NewMaterialType)
{
	MaterialType = NewMaterialType;
}

void AMineCraftCharacter::SetToolType(ETool NewToolType)
{
	ToolType = NewToolType;
}

bool AMineCraftCharacter::AddItemToInventory(class AWieldable* Item)
{
	if (Item)
	{
		const int32 AvailableSlot = Inventory.Find(nullptr);
		if (AvailableSlot != INDEX_NONE)
		{
			Inventory[AvailableSlot] = Item;
			return true;
		}
	}
	return false;
}

UTexture2D* AMineCraftCharacter::GetThumbnailAtInventorySlot(int32 Slot) const
{
	if (Inventory.Num() > 0)
	{
		if (Inventory[Slot])
		{
			return Inventory[Slot]->GetThumbnail();
		}
	}
	return nullptr;
}

 