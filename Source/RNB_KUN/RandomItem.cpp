// Fill out your copyright notice in the Description page of Project Settings.

#include "RNB_KUN.h"
#include "RandomItem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARandomItem::ARandomItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	
}

// Called when the game starts or when spawned
void ARandomItem::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComponent->SetRelativeRotation(UKismetMathLibrary::RandomRotator());
}

// Called every frame
void ARandomItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


