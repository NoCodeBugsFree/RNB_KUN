// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryAndShop/Pickup.h"
#include "Pickup_Coins.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API APickup_Coins : public APickup
{
	GENERATED_BODY()
	
public:

	/** Override base interact behavior */
	virtual void Interact_Implementation() override;
	
protected:

	/** The amount of gold for this pickup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (BlueprintProtected = "true", ClampMin = 0, ClampMax = 1000))
	int32 GoldAmount = 100;

private:	
	
	
};
