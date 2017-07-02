// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "CraftingPickup.h"

ACraftingPickup::ACraftingPickup()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(PickupMesh);

}
