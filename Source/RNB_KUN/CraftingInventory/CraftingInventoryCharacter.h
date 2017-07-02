// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CraftingInventoryCharacter.generated.h"

/** Struct describes all info about crafting(combining) items available in game */
USTRUCT(BlueprintType)
struct FCraftingInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** Is the ID of item which this item will be combined to produce Product Item  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FName ComponentID;

	/** Is the ID of item which will be created after crafting 
	*   (after combining this Item and ComponentID Item)  
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FName ProductID;

	/** Should we destroy ItemA when we craft two items ?  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	bool bDestroyItemA;

	/**  Should we destroy ItemB when we craft two items ?  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	bool bDestroyItemB;
};

/** Struct describes particular actor in game  */
USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** Item unique ID  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FName ItemID;

	/** Template class of this item  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	TSubclassOf<class ACraftingPickup> ItemPickup;

	/** Name of this item  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FText Name = FText::FromString(TEXT("Item"));

	/** Name of the action performed with this item  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FText Action = FText::FromString(TEXT("Use"));

	/** Amounts of this items  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	int32 Value = 10;

	/** Simple HUD thumbnail icon  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	UTexture2D* Thumbnail;

	/** Description of this item  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FText Description = FText::FromString(TEXT("Please enter a description"));

	/** Shows whether or not this item can be used  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	bool bCanBeUsed;

	/** Crafting Combinations with another items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	TArray<FCraftingInfo> CraftingCombinations;

	/** overloaded operator == to check equals items  */
	bool operator==(const FInventoryItem& Item) const 
	{
		if (ItemID == Item.ItemID)
		{
			return true;
		} 
		else
		{
			return false;
		}
	}
};





UCLASS()
class RNB_KUN_API ACraftingInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

	/** FPS_Camera   */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPS_Camera;
	
public:

	ACraftingInventoryCharacter();

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;
	
protected:

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** Handles moving forward/backward */
	void MoveForward(float Value);

	/** Handles stafing movement, left and right */
	void MoveRight(float Value);
	
private:

	/** Interaction distance with all interactables  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float Distance = 500.f;
	
	/** Per tick perform linetrace to check for interactables  */
	void CheckForInteractables();
		
public:

	/** Returns Fps camera component **/
	FORCEINLINE class UCameraComponent* GetFPS_Camera() const { return FPS_Camera; }
};
