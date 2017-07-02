// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CraftingInventoryGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API ACraftingInventoryGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	/** Returns Data Table  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	UDataTable* GetItemDB() const { return ItemDB; }

protected:

	/** Data table that represents all items in this game  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	class UDataTable* ItemDB;	
	
};
