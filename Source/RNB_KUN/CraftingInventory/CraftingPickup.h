// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraftingInventory/InteractableActor.h"
#include "CraftingPickup.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API ACraftingPickup : public AInteractableActor
{
	GENERATED_BODY()

public:

	ACraftingPickup();

	/** Calls to perform all BP defined "OnUsed" actions  */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void OnUsed();

	/** Calls to perform all BP defined "OnDrop" actions  */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void OnDrop();
	
protected:



private:
	
	/* Represents Pickup Mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;
	
	/**  Item unique ID */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	FName ItemID = FName("Enter ItemID...");
	
	
};
