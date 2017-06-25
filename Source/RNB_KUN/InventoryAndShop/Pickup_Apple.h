// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryAndShop/Pickup.h"
#include "Pickup_Apple.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API APickup_Apple : public APickup
{
	GENERATED_BODY()
	
public:
	
	/** override base class logic here  */
	virtual void Use_Implementation() override;

protected:



private:	
	
	
};
