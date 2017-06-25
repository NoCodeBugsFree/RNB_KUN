// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RNB_KUN.h"
#include "RNB_KUNGameMode.h"
#include "RNB_KUNHUD.h"
#include "RNB_KUNCharacter.h"
#include "Blueprint/UserWidget.h"

ARNB_KUNGameMode::ARNB_KUNGameMode()
	: Super()
{
	// use our custom HUD class
	HUDClass = ARNB_KUNHUD::StaticClass();
}

void ARNB_KUNGameMode::BeginPlay()
{
	Super::BeginPlay();

	/** Create Stamina Widget  */
	CreateStaminaWidget();

	/** Creates and sets Inventory Widget accordingly to HUD State */
	ApplyHUDChanges();
}

void ARNB_KUNGameMode::CreateStaminaWidget()
{
	ARNB_KUNCharacter* MyCharacter = Cast<ARNB_KUNCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerHUDClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ARNB_KUNGameMode::ApplyHUDChanges()
{
	// remove the previous HUD , since we're applying a new one
	if (CurrentActiveWidget)
	{
		CurrentActiveWidget->RemoveFromParent();
	}

	switch (CurrentHUDState)
	{
		/** In Game  */
		case EHUDState::HS_InGame :	
		{
			ApplyHUD(InGameHUDClass, false, false);
			break;
		}
		
		/** Inventory  */
		case EHUDState::HS_Inventory:
		{
			ApplyHUD(InventoryHUDClass, true, true);
			break;
		}
		
		/** Shop General */
		case EHUDState::HS_ShopGeneral:
		{
			ApplyHUD(ShopGeneralHUDClass, true, true);
			break;
		}
		
		/** Shop Weapon  */
		case EHUDState::HS_ShopWeapon:
		{
			//ApplyHUD(, true, true);
			break;
		}
		
		default: /** In Game  */
		{
			ApplyHUD(InGameHUDClass, false, false);
		}
		
	}
}

void ARNB_KUNGameMode::ChangeHUDState(EHUDState NewState)
{
	CurrentHUDState = NewState;
	ApplyHUDChanges();
}

bool ARNB_KUNGameMode::ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool bEnableClickEvents)
{
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (WidgetToApply)
	{
		// set mouse visibility and click events according to params
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = bShowMouseCursor;
			PlayerController->bEnableClickEvents = bEnableClickEvents;
		}

		// Create the widget 
		CurrentActiveWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);
		if (CurrentActiveWidget)
		{
			CurrentActiveWidget->AddToViewport();
			return true;
		}
	}

	return false;
}
