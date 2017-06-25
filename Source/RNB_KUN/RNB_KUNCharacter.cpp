// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RNB_KUN.h"
#include "RNB_KUNCharacter.h"
#include "RNB_KUNProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Engine.h"
#include "MySaveGame.h"
#include "Target.h"
#include "TargetForPenetrable.h"
#include "PenetrableMaterial.h"
#include "RNB_KUN/4125/Bullet.h"
#include "RNB_KUNGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ARNB_KUNCharacter

ARNB_KUNCharacter::ARNB_KUNCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
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

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ARNB_KUNCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	/** custom rifle  */
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Gun")); //Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
	FP_Gun->SetWorldScale3D(FVector(3.5f, 3.5f, 3.5f));

	/** sets the pickups inventory size */
	PickupsInventory.SetNum(4);
}

void ARNB_KUNCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/** Stamina  */
	HandleStaminaThisFrame(DeltaTime);
	
	/**  Interactable checking  */
	CheckForInteractable();
}

void ARNB_KUNCharacter::HandleStaminaThisFrame(float DeltaTime)
{
	// deplete stamina if really sprinting
	if (bIsSprinting && GetCharacterMovement()->Velocity.Size() > 10)
	{
		UpdateCurrentStamina(GetWorld()->GetDeltaSeconds() * -0.1f * InitialStamina);
	}
	else
	{
		// add some stamina whenever not sprinting
		UpdateCurrentStamina(DeltaTime * 0.01f * InitialStamina);
	}
}

void ARNB_KUNCharacter::ResetCanFire()
{
	bCanFire = true;
	GetWorldTimerManager().ClearTimer(FireDelayTimerHandle);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARNB_KUNCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &ARNB_KUNCharacter::BeginSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ARNB_KUNCharacter::EndSprint);

	// Pickup
	InputComponent->BindAction("Pickup", IE_Pressed, this, &ARNB_KUNCharacter::BeginPickup);
	InputComponent->BindAction("Pickup", IE_Released, this, &ARNB_KUNCharacter::EndPickup);

	// ShowInventory
	InputComponent->BindAction("ShowInventory", IE_Pressed, this, &ARNB_KUNCharacter::ShowInventory);

	// Interact
	InputComponent->BindAction("Interact", IE_Pressed, this, &ARNB_KUNCharacter::BeginInteract);
	InputComponent->BindAction("Interact", IE_Released, this, &ARNB_KUNCharacter::EndInteract);

	// Save the game and Load the game
	InputComponent->BindAction("SaveGame", IE_Released, this, &ARNB_KUNCharacter::SaveGame);
	InputComponent->BindAction("LoadGame", IE_Released, this, &ARNB_KUNCharacter::LoadGame);

	// RayCast
	InputComponent->BindAction("RayCast", IE_Released, this, &ARNB_KUNCharacter::PerformRayCast);

	// NewInteractable and Inventory (Inventory Tutorial)
	InputComponent->BindAction("ToggleInventory", IE_Released, this, &ARNB_KUNCharacter::ToggleInventory);
	InputComponent->BindAction("Interact", IE_Released, this, &ARNB_KUNCharacter::Interact);

	//
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARNB_KUNCharacter::TouchStarted);
	if (EnableTouchscreenMovement(InputComponent) == false)
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &ARNB_KUNCharacter::OnFire);
	}

	InputComponent->BindAxis("MoveForward", this, &ARNB_KUNCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARNB_KUNCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ARNB_KUNCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ARNB_KUNCharacter::LookUpAtRate);
}

void ARNB_KUNCharacter::OnFire()
{	
	// fire limit
	if (!bCanFire) { return; }
	else
	{
		// disable firing ability
		bCanFire = false;

		// start timer
		GetWorld()->GetTimerManager().SetTimer(FireDelayTimerHandle, this, &ARNB_KUNCharacter::ResetCanFire, FireDelay, false);
	}
	
	// ----------------------------------------------------------------
	// Bullet Penetration 
	// ----------------------------------------------------------------
	{	
		// initialize hit results array
		TArray<FHitResult> HitResults;

		// get start and end traces
		FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
		FVector EndTrace = 5000 * FirstPersonCameraComponent->GetForwardVector() + StartTrace;

		// Setup collision query and collision response params
		FCollisionQueryParams TraceParams;
		FCollisionResponseParams PesponseParams(ECollisionResponse::ECR_Overlap);

		// perform line trace
		GetWorld()->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, ECC_Visibility, TraceParams, PesponseParams);

		int32 DamageAmount = 30;

		//
		if (HitResults.Num() > 0)
		{
			for (int32 Count = 0; Count < HitResults.Num(); ++Count)
			{
				CreateBulletHole(HitResults[Count]);
			}
		}
		
		// process each hit result in our array
		for (int32 x = 0; x != HitResults.Num(); ++x)
		{
			// Check if we have hit a solid wall, if we have we can stop processing hit results
			AStaticMeshActor* PotentialSolidMaterial = Cast<AStaticMeshActor>(HitResults[x].Actor.Get());
			if (PotentialSolidMaterial && !PotentialSolidMaterial->IsPendingKill())
			{
				break; // if true - then break the loop
			}

			// Check if we have hit a penetrable material
			APenetrableMaterial* PotentialPenetrableMaterial = Cast<APenetrableMaterial>(HitResults[x].Actor.Get());
			if (PotentialPenetrableMaterial && !PotentialPenetrableMaterial->IsPendingKill())
			{
				DamageAmount = DamageAmount / PotentialPenetrableMaterial->MaterialResistance;
			}

			// Check if we have hit a target. If we have apply damage
			ATargetForPenetrable* PotentialTarget = Cast<ATargetForPenetrable>(HitResults[x].Actor.Get());
			if (PotentialTarget && !PotentialTarget->IsPendingKill())
			{
				PotentialTarget->DamageTarget(DamageAmount);
			}
		}
	}
	// ----------------------------------------------------------------
	// Bullet Penetration End
	// ----------------------------------------------------------------
	
	
	FHitResult HitResult;
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = (ForwardVector * 5000.f + StartTrace);
	FCollisionQueryParams TraceParams; 

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, 10.f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("You hit %s"), *HitResult.Actor->GetName()));
		ATarget* TestTarget = Cast<ATarget>(HitResult.Actor.Get());

		if (TestTarget && !TestTarget->IsPendingKill())
		{
			TestTarget->DamageTarget(49.f); // TODO remove hardcoded value
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("You hit NOTHING...")));
	}

	/** Fire FX  */
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

void ARNB_KUNCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ARNB_KUNCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

void ARNB_KUNCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void ARNB_KUNCharacter::BeginSprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 3000;
}

void ARNB_KUNCharacter::EndSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 300;
}

void ARNB_KUNCharacter::BeginPickup()
{
	bIsPickingup = true;
}

void ARNB_KUNCharacter::EndPickup()
{
	bIsPickingup = false;
}

void ARNB_KUNCharacter::ShowInventory()
{
	for (const auto & Item : Inventory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Item %s"), *Item));
	}
}

void ARNB_KUNCharacter::BeginInteract()
{
	bIsInteracting = true;
}

void ARNB_KUNCharacter::EndInteract()
{
	bIsInteracting = false;
}

void ARNB_KUNCharacter::FireBullet() 
{
	if (BulletType)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			
			FVector SpawnLocation = FP_Gun->GetSocketLocation("ProjectileSocket");
			FRotator SpawnRotation = GetActorForwardVector().Rotation();
			
			ABullet* Spawned = World->SpawnActor<ABullet>(BulletType, SpawnLocation, SpawnRotation, SpawnParams);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Bullet Type == NULL"));
	}
}

void ARNB_KUNCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ARNB_KUNCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ARNB_KUNCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARNB_KUNCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ARNB_KUNCharacter::EnableTouchscreenMovement(class UInputComponent* InputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARNB_KUNCharacter::BeginTouch);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &ARNB_KUNCharacter::EndTouch);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ARNB_KUNCharacter::TouchUpdate);
	}
	return bResult;
}

FTransform ARNB_KUNCharacter::GetProjectileSocketTransform() const
{
	return FP_Gun ? FP_Gun->GetSocketTransform("ProjectileSocket") : FTransform();
}

void ARNB_KUNCharacter::UpdateCoins(int32 Amount)
{
	Gold += Amount;
}

bool ARNB_KUNCharacter::AddItemToInventory(APickup* Item)
{
	if (Item)
	{
		/** find first slot with NULL - first empty slot */
		const int32 AvailableSlot = PickupsInventory.Find(nullptr);
		if (AvailableSlot != INDEX_NONE)
		{
			PickupsInventory[AvailableSlot] = Item;
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("You cant carry any more items"));
			return false;
		}
	}
	return false;
}

UTexture2D* ARNB_KUNCharacter::GetThumbnailAtInventorySlot(int32 Slot)
{
	if (PickupsInventory[Slot])
	{
		return PickupsInventory[Slot]->PickupThumbnail;
	}

	return nullptr;
}

FString ARNB_KUNCharacter::GetItemNameAtInventorySlot(int32 Slot)
{
	if (PickupsInventory[Slot])
	{
		return PickupsInventory[Slot]->ItemName;
	}
	return FString("None");
}

void ARNB_KUNCharacter::UseItemAtInventorySlot(int32 Slot)
{
	if (PickupsInventory[Slot])
	{
		PickupsInventory[Slot]->Use();
		PickupsInventory[Slot] = nullptr;
	}
}

float ARNB_KUNCharacter::GetInitialStamina() const
{
	return InitialStamina;
}

float ARNB_KUNCharacter::GetCurrentStaminaInPercent() const
{
	return CurrentStamina / 100.f;
}

float ARNB_KUNCharacter::UpdateCurrentStamina(float Stamina)
{
	CurrentStamina += Stamina;
	return CurrentStamina;
}

void ARNB_KUNCharacter::SaveGame()
{
	// Create an instance of save game class
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	// Set the save game instance location equal to the players current location
	SaveGameInstance->PlayerLocation = GetActorLocation();

	// Save the SaveGameInstance
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);
}

void ARNB_KUNCharacter::LoadGame()
{
	// Create an instance of save game class
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	// load the save game into our SaveGameInstance variable
	SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MySlot"), 0));

	// Set the players location from the saved game files
	if (SaveGameInstance)
	{
		SetActorLocation(SaveGameInstance->PlayerLocation);
	}
}

void ARNB_KUNCharacter::PerformRayCast()
{
	FHitResult HitResult;
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndTrace = (ForwardVector * 5000.f + StartTrace);
	FCollisionQueryParams TraceParams;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, true);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("You hit %s"), *(HitResult.Actor->GetName()) ));
		if (HitResult.GetActor())
		{
			HitResult.GetActor()->Destroy();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("You hit NOTHING...")));
	}
}

void ARNB_KUNCharacter::CreateBulletHole(FHitResult& HitResult)
{
	if (BulletHoleDecal)
	{
		FVector BulletHoleSize = FVector(3.5f, 7.f, 7.f);
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletHoleDecal, BulletHoleSize, HitResult.ImpactPoint, HitResult.Normal.Rotation(), 10.f);
	}
}

void ARNB_KUNCharacter::ToggleInventory()
{
	ARNB_KUNGameMode* RNB_KUNGameMode = Cast<ARNB_KUNGameMode>(GetWorld()->GetAuthGameMode());
	if (RNB_KUNGameMode)
	{
		if (RNB_KUNGameMode->GetHUDState() == EHUDState::HS_InGame)
		{
			RNB_KUNGameMode->ChangeHUDState(EHUDState::HS_Inventory);
		}
		else
		{
			RNB_KUNGameMode->ChangeHUDState(EHUDState::HS_InGame);
		}
	}
}

 void ARNB_KUNCharacter::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact();
	}
}

void ARNB_KUNCharacter::CheckForInteractable()
{
	FVector StartTrace = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector EndTrace = (GetFirstPersonCameraComponent()->GetForwardVector() * Reach) + StartTrace;

	FHitResult HitResult;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this); // ignore the actor

	// cast the line trace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECollisionChannel::ECC_WorldDynamic, CollisionQueryParams);

	AInteractable* PotentialInteractable = Cast<AInteractable>(HitResult.GetActor());
	if (PotentialInteractable == NULL)
	{
		HelpText = FString("");
		CurrentInteractable = nullptr;
		return;
	}
	else
	{
		CurrentInteractable = PotentialInteractable;
		HelpText = PotentialInteractable->GetHeplText();
	}
	
}

