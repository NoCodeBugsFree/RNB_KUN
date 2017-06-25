// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Engine.h"

/** Tool type  */
UENUM(BlueprintType)
enum class ETool : uint8
{
	E_Unarmed      UMETA(DisplayName = "Unarmed"),
	E_PickAxe      UMETA(DisplayName = "PickAxe"),
	E_Axe	       UMETA(DisplayName = "Axe"),
	E_Shovel       UMETA(DisplayName = "Shovel"),
	E_Sword		   UMETA(DisplayName = "Sword")
};

/** Material type  */
UENUM(BlueprintType)
enum class EMaterial : uint8
{
	M_None    = 1  UMETA(DisplayName = "None"),
	M_Wooden  = 2  UMETA(DisplayName = "Wooden"),
	M_Stone   = 4  UMETA(DisplayName = "Stone"),
	M_Iron    = 6  UMETA(DisplayName = "Iron"),
	M_Diamond = 8  UMETA(DisplayName = "Diamond"),
	M_Golden  = 12 UMETA(DisplayName = "Golden"),
};

/** HUD State  */
UENUM(BlueprintType)
enum class EMineCraftHUDState : uint8
{
	HS_InGame UMETA(DisplayName = "InGame"),
	HS_Inventory UMETA(DisplayName = "Inventory"),
	HS_CraftMenu UMETA(DisplayName = "CraftMenu"),
};