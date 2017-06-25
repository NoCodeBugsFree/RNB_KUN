// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "RNB_KUNHUD.generated.h"

UCLASS()
class ARNB_KUNHUD : public AHUD
{
	GENERATED_BODY()

public:

	ARNB_KUNHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

