// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RNB_KUN.h"
#include "MineCraftCharacter.generated.h"

UCLASS()
class RNB_KUN_API AMineCraftCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_WeildedItem;

public:

	// Sets default values for this character's properties
	AMineCraftCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Sets character's current material type for the wielded item */
	void SetMaterialType(EMaterial NewMaterialType);

	/** Sets character's current tool type for the wielded item */
	void SetToolType(ETool NewToolType);

	// --------- Inventory ------------------------------------------------------------

	/** Adds and item to inventory  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool AddItemToInventory(class AWieldable* Item);

	/** Returns current inventory slot  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	int32 GetCurrentInventorySlot() const { return CurrentInventorySlot; }

	/** Returns thumbnail picture for inventory slot  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	UTexture2D* GetThumbnailAtInventorySlot(int32 Slot) const;

	/** Sets the skeletal mesh, shows or hides a Wieldable accordingly to Current Inventory Slot   */
	void UpdateWieldedItem();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Handles moving forward/backward */
	void MoveForward(float Value);

	/** Handles stafing movement, left and right */
	void MoveRight(float Value);

	// -----------------------------------------------------------------------------------

	/** Weapon delay timer  */
	FTimerHandle FireTimer;

	/** Time to delay to next fire shot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	float FireDelay = 0.1f;

	/** Shows whether character can fire or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AAA")
	uint32 bIsFiring : 1;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	class UAnimMontage* FireAnimation;
	
private:
	
	/** Current Block reference */
	UPROPERTY()
	class ABlock* CurrentBlock;

	/** Interaction distance for character  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float Reach = 250.f;

	/** True if player is breaking this block, false otherwise  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	uint32 bIsBrealing : 1;

	/** Timer  */
	FTimerHandle BlockBreakingTimer;
	FTimerHandle HitAnimationTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	ETool ToolType = ETool::E_Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	EMaterial MaterialType = EMaterial::M_None;

	// ----- Inventory -----------------------------------------------------------

	/** Our inventory has only 10 slots  */
	const int32 NUM_OF_INVENTORY_SLOTS = 10;

	/** Current Inventory Slot  */
	int32 CurrentInventorySlot;

	/** Storage for character's inventory  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TArray<class AWieldable*> Inventory;
	
	/** Increment Current Inventory Slot  */
	void MoveUpInventorySlot();

	/** Decrement Current Inventory Slot  */
	void MoveDownInventorySlot();

	// -----------------------------------------------------------------------------------

	/** Calls when linetrace is successful and we need to damage a block  */
	void OnHit();

	/** Calls when we need to stop damage the block  */
	void EndHit();

	/** Plays hit anims and sound FX if they are specified */
	void PlayHitAnimation();

	/** Calls to break a block  */
	void BreakBlock();

	/** Call to check whether or not we have a block in front of a player at specified distance  */
	void CheckForBlocks();

	/** Returns Current Wieldable Item reference */
	class AWieldable* GetCurrentWieldableItem() const;

	/** Throw Current item into the World  */
	void Throw();

public:

	/** Returns First Person Camera Component subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Returns FP_WeildedItem subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetFP_WeildedItem() const { return FP_WeildedItem; }
	
};
