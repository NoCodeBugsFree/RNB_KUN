// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "RNB_KUNGameMode.generated.h"

UENUM(BlueprintType)
enum class EHUDState : uint8
{
	HS_InGame UMETA(DisplayName = "InGame"),
	HS_Inventory UMETA(DisplayName = "Inventory"),
	HS_ShopGeneral UMETA(DisplayName = "ShopGeneral"),
	HS_ShopWeapon UMETA(DisplayName = "ShopWeapon"),
};

UCLASS(minimalapi)
class ARNB_KUNGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ARNB_KUNGameMode();

	virtual void BeginPlay() override;

	void CreateStaminaWidget();
	
	/* checks the hud state and then calls apply HUD accordingly to apply whatever HUD we want */
	void ApplyHUDChanges();

	/* Returns current HUD State */
	EHUDState GetHUDState() const { return CurrentHUDState; }

	/* Sets the new state and then calls ApplyHUDChanges() function */
	UFUNCTION(BlueprintCallable, Category = HUDFunctions)
	void ChangeHUDState(EHUDState NewState);

	/*  Apply a HUD to the screen , return true if success or false otherwise */
	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool bEnableClickEvents);
	
protected:

	/** Current HUD State  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	EHUDState CurrentHUDState = EHUDState::HS_InGame;

	/* The HUD to be shown in game*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HUDWidgets, Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> InGameHUDClass;

	/* The HUD to be shown in the inventory*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HUDWidgets, Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> InventoryHUDClass;

	/* The HUD to be shown in the shop*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HUDWidgets, Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> ShopGeneralHUDClass;
	
	// ------ Stamina ----------------------------------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stamina, Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> PlayerHUDClass;

	/** Stamina Widget Reference */
	UPROPERTY()
	class UUserWidget* CurrentWidget;

	/** Inventory Widget Reference */
	UPROPERTY() 
	class UUserWidget* CurrentActiveWidget;
};



