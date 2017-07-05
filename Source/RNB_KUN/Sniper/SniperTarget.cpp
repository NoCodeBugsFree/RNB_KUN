// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "SniperTarget.h"


// Sets default values
ASniperTarget::ASniperTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASniperTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASniperTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

