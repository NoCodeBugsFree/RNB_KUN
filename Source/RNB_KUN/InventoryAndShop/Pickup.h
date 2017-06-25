// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InventoryAndShop/Interactable.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API APickup : public AInteractable
{
	GENERATED_BODY()


public:
	
	APickup();

	virtual void BeginPlay() override;
	
	virtual void Interact_Implementation() override;

	/** Calls to handle actors behavior when it was picked up  */
	void OnPickedup();

	/** Calls when we need to use this item  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void Use();
	virtual void Use_Implementation();

	/** Amount this items  */
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	int32 Value = 0;

	/** Item Name to display during the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	FString ItemName = FString("Enter the Item name here...");

	/** Represents pickup thumbnail   */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UTexture2D* PickupThumbnail;

};
