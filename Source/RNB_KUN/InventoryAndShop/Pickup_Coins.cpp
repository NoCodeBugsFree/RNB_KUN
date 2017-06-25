// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "Pickup_Coins.h"
#include "RNB_KUNCharacter.h"

void APickup_Coins::Interact_Implementation()
{
	ARNB_KUNCharacter* RNB_KUNCharacter = Cast<ARNB_KUNCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (RNB_KUNCharacter && !IsPendingKill())
	{
		RNB_KUNCharacter->UpdateCoins(GoldAmount);
		Destroy();
	}
}
