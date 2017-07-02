// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "CrafringPlayerController.h"
#include "CraftingInventory/InteractableActor.h"
#include "CraftingInventory/CraftingInventoryGameMode.h"
#include "CraftingInventory/CraftingInventoryCharacter.h"

void ACrafringPlayerController::SetCurrentInteractable(AInteractableActor* InteractableActor)
{
	CurrentInteractable = InteractableActor;
}

void ACrafringPlayerController::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);
	}
}

void ACrafringPlayerController::AddItemToInventoryByID(FName ID)
{
	
	ACraftingInventoryGameMode* CraftingInventoryGameMode = Cast<ACraftingInventoryGameMode>(GetWorld()->GetAuthGameMode());
	if (CraftingInventoryGameMode)
	{
		UDataTable* ItemTable = CraftingInventoryGameMode->GetItemDB();
		if (ItemTable)
		{
			/** Function to find the row of a table given its name. */
			FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");
			if (ItemToAdd)
			{
				Inventory.Add(*ItemToAdd);
			}
		}

	}
}

void ACrafringPlayerController::CraftItem(FInventoryItem ItemA, FInventoryItem ItemB, ACrafringPlayerController* Controller)
{
	// check if we've made a valid craft 
	for (FCraftingInfo Craft : ItemB.CraftingCombinations)
	{
		if (Craft.bDestroyItemA)
		{
			Inventory.RemoveSingle(ItemA);
		}
		if (Craft.bDestroyItemB)
		{
			Inventory.RemoveSingle(ItemB);
		}

		AddItemToInventoryByID(Craft.ProductID);

		ReloadInventory();
	}
}

void ACrafringPlayerController::RemoveItemFrominventory(FInventoryItem ItemToRemove)
{
	int32 Index;
	if (Inventory.Find(ItemToRemove, Index))
	{
		if (Index != INDEX_NONE)
		{
			Inventory.RemoveAt(Index);
		}
	}
}

void ACrafringPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &ACrafringPlayerController::Interact);
}
