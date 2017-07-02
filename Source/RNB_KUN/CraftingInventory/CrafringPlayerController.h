// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CraftingInventory/CraftingInventoryCharacter.h"
#include "CrafringPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API ACrafringPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/** Sets the current interactable if we are looking at it  */
	void SetCurrentInteractable(class AInteractableActor* InteractableActor);

	/** If we have a  Current Interactable - call its interact function  */
	void Interact();

	/** Adds item to inventory if it described in GameMode Data table  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void AddItemToInventoryByID(FName ID);

	/** Attempt to craft two items if it is possible   */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void CraftItem(FInventoryItem ItemA, FInventoryItem ItemB, ACrafringPlayerController* Controller);

	/** For each item in inventory creates a widget-slot and adds they to inventory HUD  */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void ReloadInventory();

	/** Removes given item from inventory  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void RemoveItemFrominventory(FInventoryItem ItemToRemove);

protected:

	virtual void SetupInputComponent() override;

private:

	/** Represents character's inventory  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TArray<FInventoryItem> Inventory;

	/** Current Interactable actor reference  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class AInteractableActor* CurrentInteractable;
};
