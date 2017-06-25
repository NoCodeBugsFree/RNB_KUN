// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "Block.h"


// Sets default values
ABlock::ABlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SM_BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_BlockMesh"));
	SM_BlockMesh->SetupAttachment(RootComponent);
}

void ABlock::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	DynamicMaterial = SM_BlockMesh->CreateDynamicMaterialInstance(0);
}

void ABlock::ResetBlock()
{
	BreakingStages = 0.f;

	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue("CrackingValue", 1.f);
	}
}

void ABlock::Break()
{
	++BreakingStages; 
	float CrackingValue = 1.f - BreakingStages / 5.f; // 5.f - amount of stages

	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue("CrackingValue", CrackingValue);
	}

	if (BreakingStages >= 5.f)
	{
		OnBroken(true);
	}
}

void ABlock::OnBroken(bool HasRequiredPickAxe)
{
	Destroy();
}
