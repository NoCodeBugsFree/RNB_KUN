// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class RNB_KUN_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere)
	FVector PlayerLocation;
	
	
};
