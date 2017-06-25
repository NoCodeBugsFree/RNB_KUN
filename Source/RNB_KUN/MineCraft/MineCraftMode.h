// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MineCraftMode.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API AMineCraftMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

protected:

	/** The Current HUD state  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (BlueprintProtected = "true"))
	EMineCraftHUDState CurrentHUDState = EMineCraftHUDState::HS_InGame;

	/** Template HUD for using "in Inventory"  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> InventoryHUD;

	/** Template HUD for using "in Game"  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> InGameHUD;

	/** Template HUD for using "in CraftMenu"  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> CraftMenuHUD;
	
private:

	/** Current HUD reference  */
	UPROPERTY()
	class UUserWidget* CurrentWidget;
	
public:

	/** Returns current HUD state  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	EMineCraftHUDState GetHUDState() const { return CurrentHUDState; }

	/** Sets new HUD State and apply changes  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ChangeHUDState(EMineCraftHUDState NewHUDState);

	/** Apply new HUD accordingly to Current HUD State  */
	void ApplyHUDChanges();

	/**   */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool ApplyHUD(TSubclassOf<UUserWidget> WidgetToApply, bool bShowMouseCursor, bool bEnableClickEvents);

};
