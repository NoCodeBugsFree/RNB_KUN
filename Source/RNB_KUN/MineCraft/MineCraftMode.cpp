// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "MineCraftMode.h"
#include "Blueprint/UserWidget.h"
#include "MineCraftCharacter.h"

void AMineCraftMode::BeginPlay()
{
	Super::BeginPlay();

	ApplyHUDChanges();
}

void AMineCraftMode::ChangeHUDState(EMineCraftHUDState NewHUDState)
{
	CurrentHUDState = NewHUDState;
	ApplyHUDChanges();
}

void AMineCraftMode::ApplyHUDChanges()
{
	/** Remove old widget  */
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}

	/** Apply new HUD accordingly to Current HUD State  */
	switch (CurrentHUDState)
	{
	case EMineCraftHUDState::HS_InGame:
		ApplyHUD(InGameHUD, false, false);
		break;

	case EMineCraftHUDState::HS_Inventory:
		ApplyHUD(InventoryHUD, true, true );
		break;

	case EMineCraftHUDState::HS_CraftMenu:
		ApplyHUD(CraftMenuHUD, true, true);
		break;

	default:
		ApplyHUD(InGameHUD, false, false);
	}	
}

bool AMineCraftMode::ApplyHUD(TSubclassOf<UUserWidget> WidgetToApply, bool bShowMouseCursor, bool bEnableClickEvents)
{
	if (WidgetToApply)
	{
		// for each MineCraftCharacter in the World
		for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; It++)
		{
			if (APlayerController* PlayerController = Cast<APlayerController>(*It))
			{
				if (AMineCraftCharacter* MineCraftCharacter = Cast<AMineCraftCharacter>(PlayerController->GetPawn()))
				{
					if (MineCraftCharacter)
					{
						PlayerController->bShowMouseCursor = bShowMouseCursor;
						PlayerController->bEnableClickEvents = bEnableClickEvents;

						/** Create the widget  */
						CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);
						if (CurrentWidget)
						{
							CurrentWidget->AddToViewport();
							return true;
						}
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetToApply == NULL"));
	}

	return false;
}