// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "InventoryAndShop/Interactable.h"
#include "InventoryAndShop/Pickup.h"
#include "RNB_KUNCharacter.generated.h"

class UInputComponent;
class APickup;

UCLASS(config = Game)
class ARNB_KUNCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

public:

	ARNB_KUNCharacter();

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;
	
	/** Calls per frame to handle stamina  */
	void HandleStaminaThisFrame(float DeltaTime);

	/** Returns bIsPickingup state  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool IsPickingup() const { return bIsPickingup; }

	/** Returns bIsInteracting state  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool IsInteracting() const { return bIsInteracting; }

	// ------------- Stamina ----------------------------------------------------------------------
	
#pragma region Stamina

	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool IsSprintig() const { return bIsSprinting; }

	/** Accessor function for initial stamina **/
	UFUNCTION(BlueprintPure, Category = Stamina)
	float GetInitialStamina() const;

	/** Accessor function for initial stamina **/
	UFUNCTION(BlueprintPure, Category = Stamina)
	float GetCurrentStaminaInPercent() const;

	/**Update the players current stamina
	* @param Stamina This is the amount to change the player stamina by. It can be either positive or negative
	**/
	UFUNCTION(BlueprintPure, Category = Stamina)
	float UpdateCurrentStamina(float Stamina);

#pragma endregion



	// ------------- Base ----------------------------------------------------------------------

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ARNB_KUNProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

protected:

	/** shows whether or not character can fire */
	bool bCanFire = true;

	/** Time delay intervals between shots  */
	UPROPERTY(EditAnywhere)
	float FireDelay = 1.f;

	/** Recharge timer  */
	FTimerHandle FireDelayTimerHandle;

	/** Enable fire ability after recharge  */
	void ResetCanFire();

	/** Fires a projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};

	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

	/** Handles player sprinting. */
	UFUNCTION(BlueprintCallable, Category = AAA)
	void BeginSprint();

	UFUNCTION(BlueprintCallable, Category = AAA)
	void EndSprint();
	
	UFUNCTION(BlueprintCallable, Category = AAA)
	void BeginPickup();

	UFUNCTION(BlueprintCallable, Category = AAA)
	void EndPickup();

	UFUNCTION(BlueprintCallable, Category = AAA)
	void ShowInventory();

	UFUNCTION(BlueprintCallable, Category = AAA)
	void BeginInteract();

	UFUNCTION(BlueprintCallable, Category = AAA)
	void EndInteract();

	void ThrowGranade();

	// TODO don't work properly
	/** Bullet physics tutorial   */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void FireBullet();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	bool bIsInteracting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	bool bIsPickingup = false;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABullet> BulletType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	TSubclassOf<class ALauncherProjectile> LauncherProjectileToSpawn;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/*
	* Configures input for touchscreen devices if there is a valid touch interface for doing so
	*
	* @param	InputComponent	The input component pointer to bind controls to
	* @returns true if touch controls were enabled.
	*/
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	/** Returns Projectile Socket Transform data to spawn projectile  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	FTransform GetProjectileSocketTransform() const;
	
	UPROPERTY()
	TArray<FString> Inventory;

	int32 PlayerXp = 0;
	
private:
	
	/** The players initial stamina **/
	UPROPERTY(EditAnywhere, Category = Stamina)
	float InitialStamina = 50;

	/** The players current stamina **/
	UPROPERTY(EditAnywhere, Category = Stamina)
	float CurrentStamina = 50;
	
	/**  Saves the game to slot */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SaveGame();

	/** Loads the game to slot  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void LoadGame();

	/** Simple ray cast example  */
	void PerformRayCast();

	UPROPERTY(EditAnywhere, Category = BulletDecals)
	UMaterialInterface * BulletHoleDecal;

	/** This function creates a bullet hole at the location we have hit **/
	void CreateBulletHole(FHitResult& HitResult);

	// -----------------------------------------------------------------------------------
	//			Inventory tutorial
	// -----------------------------------------------------------------------------------

private:

	/** The interaction distance  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float Reach = 250.f;

	/** Toggles the Inventory - InGame/Inventory states */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ToggleInventory();

	/** Calls BPNative event Interact() on Current Interactable if its valid   */
	void Interact();

	/** Calls to perform a raycast and setup  Current Interactable on success */
	void CheckForInteractable();

	/** Current Interactable reference */
	UPROPERTY()
	AInteractable* CurrentInteractable;

	/** Player's Pickups Inventory  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TArray<APickup*> PickupsInventory;

public:

	/** Help Text which represents ... */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	FString HelpText;

	/** Current character's gold amount ... */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 Gold;

	/** Updates the character's coins amount
	* @param Amount this is the amount to update the coins by, can be
	* positive or negative
	*/
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void UpdateCoins(int32 Amount);

	/** Adds an item to the inventory  */
	UFUNCTION(BlueprintPure, Category = "AAA")
	bool AddItemToInventory(APickup* Item);

	/** Gets the Item Name for a given inventory slot */
	UFUNCTION(BlueprintPure, Category = "AAA")
	UTexture2D* GetThumbnailAtInventorySlot(int32 Slot);

	/** Gets the Thumbnail for a given inventory slot */
	UFUNCTION(BlueprintPure, Category = "AAA")
	FString GetItemNameAtInventorySlot(int32 Slot);

	/** Uses the item at inventory slot  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void UseItemAtInventorySlot(int32 Slot);

};

